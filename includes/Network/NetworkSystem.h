#pragma once
#include "Engine/ISystem.h"

class NetworkSystem : public ISystem
{
private:
    bool multiplayerEnabled = false;

public:
    NetworkSystem() = default;

    void update(float deltaTime) override {
    }

    void enableMultiplayer(bool enable) {
        multiplayerEnabled = enable;
    }

    bool isMultiplayerEnabled() const {
        return multiplayerEnabled;
    }

    void shutdown() {
        multiplayerEnabled = false;
    }
};