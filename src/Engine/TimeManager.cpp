//
// Created by kikker234 on 07-11-2025.
//

#include "Engine/TimeManager.h"

#include <chrono>

void TimeManager::start() {
    _lastFrameTime = std::chrono::high_resolution_clock::now();
}

float TimeManager::update() {
    if (_isPaused) {
        return 0.0f;
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - _lastFrameTime;
    _lastFrameTime = currentTime;

    return elapsed.count() * _timeScale;
}

float TimeManager::getTimeScale() {
    return _timeScale;
}

void TimeManager::setTimeScale(float timeScale) {
    _timeScale = timeScale;
}

bool TimeManager::isPaused() {
    return _isPaused;
}

void TimeManager::pause() {
    _isPaused = true;
}

void TimeManager::resume() {
    _isPaused = false;
}
