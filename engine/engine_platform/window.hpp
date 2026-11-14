#pragma once

#include "../engine_config.hpp"
#include "stb_image.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

// GLFW Window: Forward declaration of GLFW's window type.
struct GLFWwindow;

// Framebuffer Extent: The size of a window's framebuffer.
struct FramebufferExtent {

    // Size
    int width = 0;  // in pixels
    int height = 0;  // in pixels
};

// Window: A GLFW window with an OpenGL context.
class Window {
public:
    void windowInitialize();
    void windowClose();
    void windowShutdown();
    
    void windowPollEvents();
    void windowSwapBuffers();
    bool windowShouldClose() const;

    GLFWwindow* windowNativeHandle() const;
    bool windowConsumeFramebufferResize(FramebufferExtent& extent);

    // Framebuffer
    FramebufferExtent m_framebuffer_extent;  // latest framebuffer size, in pixels
    bool m_framebuffer_resized = false;  // true until windowConsumeFramebufferResize reads the new size

    // Scroll
    float scrollX = 0.0f;  // horizontal scroll this frame, reset by windowPollEvents
    float scrollY = 0.0f;  // vertical scroll this frame, reset by windowPollEvents

private:

    // Handle
    GLFWwindow* m_native_handle = nullptr;  // initialize as null pointer
};
