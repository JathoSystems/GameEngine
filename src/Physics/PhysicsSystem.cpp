#include "Physics/PhysicsSystem.h"
#include "Physics/PhysicsComponent.h"
#include "GameObjects/GameObject.h"
#include "Collision/CollisionData.h"
#include <box2d/box2d.h>

PhysicsSystem::PhysicsSystem() : _gravityX(0.0f), _gravityY(981.0f) {}

PhysicsSystem::~PhysicsSystem() = default;

void PhysicsSystem::init(float gravityX, float gravityY) {
    _gravityX = gravityX;
    _gravityY = gravityY;
    _box2DFacade = std::make_unique<Box2DFacade>();
    _box2DFacade->setGravity(_gravityX, _gravityY);
}

void PhysicsSystem::update(float deltaTime) {
    _box2DFacade->step(deltaTime);

    for (auto* component : _components) {
        component->update(deltaTime);
    }

    processCollisions();
}

void PhysicsSystem::processCollisions() {
    b2ContactEvents events = b2World_GetContactEvents(_box2DFacade->getWorldId());

    for (int i = 0; i < events.beginCount; ++i) {
        b2ContactBeginTouchEvent* event = events.beginEvents + i;

        b2BodyId bodyA = b2Shape_GetBody(event->shapeIdA);
        b2BodyId bodyB = b2Shape_GetBody(event->shapeIdB);

        GameObject* objA = static_cast<GameObject*>(b2Body_GetUserData(bodyA));
        GameObject* objB = static_cast<GameObject*>(b2Body_GetUserData(bodyB));

        if (objA && objB) {
            CollisionData dataA(objB, 0.0f, -1.0f, true);
            CollisionData dataB(objA, 0.0f, 1.0f, true);

            objA->onCollisionEnter(dataA);
            objB->onCollisionEnter(dataB);
        }
    }

    for (int i = 0; i < events.endCount; ++i) {
        b2ContactEndTouchEvent* event = events.endEvents + i;

        b2BodyId bodyA = b2Shape_GetBody(event->shapeIdA);
        b2BodyId bodyB = b2Shape_GetBody(event->shapeIdB);

        GameObject* objA = static_cast<GameObject*>(b2Body_GetUserData(bodyA));
        GameObject* objB = static_cast<GameObject*>(b2Body_GetUserData(bodyB));

        if (objA && objB) {
            CollisionData dataA(objB, 0.0f, 0.0f, false);
            CollisionData dataB(objA, 0.0f, 0.0f, false);

            objA->onCollisionExit(dataA);
            objB->onCollisionExit(dataB);
        }
    }
}

void PhysicsSystem::registerComponent(PhysicsComponent* component) {
    _components.push_back(component);
}

void PhysicsSystem::unregisterComponent(PhysicsComponent* component) {
    _components.erase(
        std::remove(_components.begin(), _components.end(), component),
        _components.end()
    );
}

void PhysicsSystem::setGravity(float x, float y) {
    _gravityX = x;
    _gravityY = y;
    _box2DFacade->setGravity(x, y);
}

void PhysicsSystem::getGravity(float& x, float& y) const {
    x = _gravityX;
    y = _gravityY;
}
