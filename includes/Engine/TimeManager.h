#ifndef GAMEENGINE_TIMEMANAGER_H
#define GAMEENGINE_TIMEMANAGER_H
#include <chrono>

class TimeManager {
private:
    float _timeScale = 1;
    bool _isPaused = false;
    std::chrono::high_resolution_clock::time_point _lastFrameTime;

public:
    float getTimeScale();

    void setTimeScale(float timeScale);

    bool isPaused();

    void pause();

    void resume();

    void start();

    float update();
};

#endif //GAMEENGINE_TIMEMANAGER_H
