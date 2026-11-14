#include "time.hpp"

// Timer Initialize: Starts the timer. Call once after GLFW is initialized.
void Timer::timerInitialize() {
    m_start_time = glfwGetTime();
    m_previous_time = m_start_time;
    m_current_time = m_start_time;
    m_delta_time = 0.0f;
}

// Timer Update: Updates the delta time. Call once per frame.
void Timer::timerUpdate() {
    m_current_time = glfwGetTime();
    m_delta_time = static_cast<float>(m_current_time - m_previous_time);
    m_previous_time = m_current_time;
}

// Timer Delta Time: Returns the seconds between the last two timerUpdate calls.
float Timer::timerDeltaTime() const {
    return m_delta_time;
}

// Timer Elapsed Time: Returns the seconds from timerInitialize to the latest timerUpdate.
float Timer::timerElapsedTime() const {
    return static_cast<float>(m_current_time - m_start_time);
}
