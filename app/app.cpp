#include "app.hpp"

void Application::initialize() {
    m_window.windowInitialize();
    m_timer.timerInitialize();
    graphicsContextInitialize();
    m_input.inputAttachWindow(m_window);

    // Place Initialization Logic Here:

    m_screen_manager.initialize();  // initialize screen manager
    m_screen_manager.screen_ptr->initialize();  // initialize screen

    // -
}

void Application::processInput() {
    m_window.windowPollEvents();
    m_input.inputUpdate();

    // Place Input Logic Here:

    developerControls(m_input, m_screen_manager);
    userControls(m_input, m_screen_manager, m_controls_state);

    if (m_screen_manager.screen_ptr) m_screen_manager.screen_ptr->processInput(m_input, m_controls_state);

    // -
}

void Application::update(float& delta_time) {

    // Place Update Logic Here:

    m_screen_manager.changeScreen(m_controls_state);
    if (m_screen_manager.screen_ptr) m_screen_manager.screen_ptr->update(delta_time);

    // -
}

void Application::render() {
    graphicsContextBeginFrame();
    graphicsContextBeginOverlayPass();

    // Place Render Logic Here:

    if (m_screen_manager.screen_ptr) m_screen_manager.screen_ptr->render();

    // -

    graphicsContextEndFrame();
}

void Application::shutdown() {

    // Place Shutdown Logic Here:

    if (m_screen_manager.screen_ptr) m_screen_manager.screen_ptr->shutdown(m_controls_state);
    
    // -

    m_window.windowShutdown();
}

void Application::run() {
    initialize();

    running = true;
    while (!m_window.windowShouldClose()) {
        m_timer.timerUpdate();
        float delta_time = m_timer.timerDeltaTime();

        processInput();
        update(delta_time);
        render();

        m_window.windowSwapBuffers();
    }
    shutdown();
}
