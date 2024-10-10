#include <SDL2/SDL.h>

class Clock {
public:
    void start(int timer);

    void stop();

    void pause();

    void unpause();

    bool ticktock(const int& timer);

    int m_timer{0};

     int m_currentTime{0};

private:

    int getTime();

    int m_startTime{0};

    bool isPause{false};

    bool isStart{false};
};
