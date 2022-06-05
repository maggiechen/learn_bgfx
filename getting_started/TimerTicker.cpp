#include "TimerTicker.h"
int64_t TimerTicker::m_currentTime = 0;
int64_t TimerTicker::m_deltaTime = 0;

void TimerTicker::Tick() {
    int64_t last = m_currentTime;
    m_currentTime = bx::getHPCounter();
    m_deltaTime = m_currentTime - last;
}

float TimerTicker::GetDeltaTime() {
    return (float)m_deltaTime / double(bx::getHPFrequency());
}
