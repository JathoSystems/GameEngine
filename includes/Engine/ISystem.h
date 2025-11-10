//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_ISYSTEM_H
#define GAMEENGINE_ISYSTEM_H

class ISystem {
public:
    virtual ~ISystem() = default;

    virtual void update(float deltaTime) = 0;
};

#endif //GAMEENGINE_ISYSTEM_H