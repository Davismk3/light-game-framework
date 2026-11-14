#include "window.hpp"

/*
C++ Refresher
int* ptr_to_var = &var          a pointer to var's address
var = *ptr_to_var               var from the pointer to its address
int& ref_to_var = var           reference to var, shorthand for (*ptr_to_var)
*/

namespace {
    
    // GLFW Error Callback: Prints GLFW errors to std::cerr. error: GLFW's error code. description: GLFW's error message.
    static void glfwErrorCallback(int error, const char* description) {
        std::cerr << "[GLFW Error " << error << "] " << description << '\n';
    }

    // Initialize GLFW: Initializes GLFW and requests an OpenGL 3.3 core context. Throws if GLFW fails to initialize.
    void initializeGLFW() {
        glfwSetErrorCallback(glfwErrorCallback);

        #ifdef __APPLE__
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
        #endif

        if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }

    // Initialize GLAD: Loads the OpenGL functions. Call after the context is made current.
    void initializeGLAD() {
        gladLoadGL(glfwGetProcAddress);
    }

    // Configure OpenGL: Enables depth testing.
    void configureOpenGL() {
        glEnable(GL_DEPTH_TEST);
    }

    // Framebuffer Size Callback: Stores the new size on the Window and marks it resized. width, height: the new framebuffer size, in pixels.
    void frameBufferSizeCallback(GLFWwindow* m_window, int width, int height) {
        //glViewport(0, 0, width, height);
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(m_window));
        if (!window) return;
        window->m_framebuffer_extent = {width, height};
        window->m_framebuffer_resized = true;
    }

    // Set Icon: Loads ICON_PATH as the window's icon, or prints a message if it fails to load. native_handle: a created window.
    void setIcon(GLFWwindow* native_handle) {
        GLFWimage images[1];
        images[0].pixels = stbi_load(ICON_PATH, &images[0].width, &images[0].height, 0, 4);

        if (images[0].pixels) {
            glfwSetWindowIcon(native_handle, 1, images);
            stbi_image_free(images[0].pixels);
        } else printf("app icon failed to load\n");
    }

    // Scroll Callback: Adds the scroll to the Window's scrollX and scrollY. xoffset, yoffset: the scroll amounts from GLFW.
    void scrollCallback(GLFWwindow* m_window, double xoffset, double yoffset) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(m_window));
        window->scrollX += xoffset;
        window->scrollY += yoffset;
    }
}

// Window Initialize: Creates the window and OpenGL context, loads GLAD, enables vsync, and sets the callbacks and icon. Throws if the window cannot be created.
void Window::windowInitialize() {
    initializeGLFW();

    m_native_handle = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);

    if (!m_native_handle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    glfwMakeContextCurrent(m_native_handle);

    // Must happen before any gl* call
    initializeGLAD();

    glfwSwapInterval(1);  // max frame rate is monitor refresh rate
    // ^ setting this to zero will cause the FPS to be unbounded, which may blow up your computer, so don't do it

    int framebuffer_width = 0;
    int framebuffer_height = 0;
    glfwGetFramebufferSize(m_native_handle, &framebuffer_width, &framebuffer_height);
    m_framebuffer_extent = {framebuffer_width, framebuffer_height};
    m_framebuffer_resized = true;
    glViewport(0, 0, framebuffer_width, framebuffer_height);

    glfwSetWindowUserPointer(m_native_handle, this);
    glfwSetFramebufferSizeCallback(m_native_handle, frameBufferSizeCallback);

    glfwSetScrollCallback(m_native_handle, scrollCallback);

    configureOpenGL();

    setIcon(m_native_handle);
}

// Window Close: Asks the window to close. windowShouldClose returns true afterward.
void Window::windowClose() {
    glfwSetWindowShouldClose(m_native_handle, true);
}

// Window Shutdown: Destroys the window and terminates GLFW.
void Window::windowShutdown() {
    glfwDestroyWindow(m_native_handle);
    glfwTerminate();
}

// Window Poll Events: Resets this frame's scroll, then processes GLFW events. Call once per frame, before inputUpdate.
void Window::windowPollEvents() {
    scrollX = 0.0;
    scrollY = 0.0;
    
    glfwPollEvents();
}

// Window Swap Buffers: Shows the frame that was just drawn.
void Window::windowSwapBuffers() {
    glfwSwapBuffers(m_native_handle);
}

// Window Should Close: True once the user or windowClose has asked the window to close.
bool Window::windowShouldClose() const {
    return glfwWindowShouldClose(m_native_handle);
}

// Window Native Handle: Returns the GLFW window, or nullptr before windowInitialize.
GLFWwindow* Window::windowNativeHandle() const {
    return m_native_handle;
}

// Window Consume Framebuffer Resize: True once after each resize, then false until the next. extent: set to the new size, if there is one.
bool Window::windowConsumeFramebufferResize(FramebufferExtent& extent) {
    if (!m_framebuffer_resized) return false;

    extent = m_framebuffer_extent;
    m_framebuffer_resized = false;

    return true;
}