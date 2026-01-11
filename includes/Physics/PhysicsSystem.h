#ifndef GAMEENGINE_PHYSICSSYSTEM_H
#define GAMEENGINE_PHYSICSSYSTEM_H

#include "Engine/ISystem.h"
#include "Box2DFacade.h"
#include <memory>
#include <vector>

class PhysicsComponent;

class PhysicsSystem : public ISystem {
private:
    std::unique_ptr<Box2DFacade> _box2DFacade;
    std::vector<PhysicsComponent *> _components;
    float _gravityX;
    float _gravityY;

    void processCollisions();

public:
    PhysicsSystem();

    ~PhysicsSystem();

    void init(float gravityX = 0.0f, float gravityY = 981.0f);

    void update(float deltaTime);

    void registerComponent(PhysicsComponent *component);

    void unregisterComponent(PhysicsComponent *component);

    Box2DFacade *getBox2DFacade() { return _box2DFacade.get(); }

    void setGravity(float x, float y);

    void getGravity(float &x, float &y) const;

    void clearAllComponents();
};

#endif
