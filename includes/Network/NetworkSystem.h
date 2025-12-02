#pragma once
#include "Engine/ISystem.h"

class NetworkSystem : public ISystem
{
public:
    NetworkSystem();
    void update(float deltaTime) override;
};


