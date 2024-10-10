#include "clock.hpp"

void Clock::start(int timer) {
    isStart = true;
    m_timer = timer;
    m_startTime = SDL_GetTicks();
}

void Clock::stop()
{
    //Stop the timer
    isStart = false;

    //Unpause the timer
    isPause = false;

	//Clear tick variables
	m_startTime = 0;
}

bool Clock::ticktock(const int& timer) {
    if(!isStart) {
        start(timer);
    }
    int time_left = timer - (SDL_GetTicks() - m_startTime);
    m_currentTime = time_left / 1000;
    if(time_left <= 0) {
        stop();
        return false;
    }
    return true;
}

