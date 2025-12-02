#pragma once
#include "Engine/ISystem.h"

class NetworkSystem : public ISystem
{
private:
    bool gameMode;
public:
    NetworkSystem(bool gameMode) : gameMode(gameMode) {};
    bool getGameMode();
    void update(float deltaTime) override;
};


