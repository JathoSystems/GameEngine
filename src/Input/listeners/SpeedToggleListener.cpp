#include "Input/listeners/SpeedToggleListener.hpp"
#include "Engine/GameEngine.h"

void SpeedToggleListener::onKeyPress(Key key) {
    GameEngine &engine = GameEngine::getInstance();
    TimeManager *time = engine.getTimeManager();

    switch (key) {
        case Key::PAGE_UP:
            if (time->getTimeScale() < 4.0f) {
                time->setTimeScale(time->getTimeScale() + 0.25f);
            }
            break;

        case Key::PAGE_DOWN:
            if (time->getTimeScale() > 0.25f) {
                time->setTimeScale(time->getTimeScale() - 0.25f);
            }
            break;

        case Key::HOME:
            time->setTimeScale(1.0f);
            break;

        default:
            break;
    }
}

void SpeedToggleListener::onKeyRelease(Key key) {
    return;
}
