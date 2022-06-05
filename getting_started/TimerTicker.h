#pragma once
#include <bx/timer.h>

class TimerTicker {
private:
    static int64_t m_currentTime;
    static int64_t m_deltaTime;
public:
    /// @brief Update time internally
    static void Tick();

    /// @brief Get the last change in time between ticks
    static float GetDeltaTime();
};
