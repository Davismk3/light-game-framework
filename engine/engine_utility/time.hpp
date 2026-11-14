#pragma once

#include <GLFW/glfw3.h>

// Timer: Tracks the time between frames and since initialization, using GLFW's clock.
class Timer {
public:
    void timerInitialize();
    void timerUpdate();
    float timerDeltaTime() const;
    float timerElapsedTime() const;

private:

    // Times
    double m_start_time = 0.0;  // seconds, at timerInitialize
    double m_previous_time = 0.0;  // seconds, at the previous timerUpdate
    double m_current_time = 0.0;  // seconds, at the latest timerUpdate

    // Delta
    float m_delta_time = 0.0f;  // seconds between the last two timerUpdate calls
};
