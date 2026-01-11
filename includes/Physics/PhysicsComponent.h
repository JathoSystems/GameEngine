#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "GameObjects/Component/Component.h"
#include "RigidBody.h"
#include "Collider.h"
#include <memory>
#include <box2d/box2d.h>

class Box2DFacade;
class GameObject;

class PhysicsComponent : public Component {
private:
    RigidBody _rigidBody;
    std::unique_ptr<Collider> _collider;
    Box2DFacade* _box2DFacade;
    b2BodyId _bodyId;
    bool _initialized;

    void initializePhysicsBody();
    void syncTransformFromPhysics();

public:
    PhysicsComponent(Box2DFacade* facade);
    ~PhysicsComponent();

    void update(float deltaTime) override;
    void render(const std::unique_ptr<Window>& window) override {}

    void setParent(GameObject *game_object) override;
    void setPosition(float x, float y);
    void setBodyType(BodyType type);
    void setCollider(std::unique_ptr<Collider> collider);
    void setMaterial(const Material& material);
    void setGravityScale(float scale);
    void setFixedRotation(bool fixed);
    void setVelocity(float vx, float vy);
    void getVelocity(float& vx, float& vy);
    void applyForce(float fx, float fy);
    void applyImpulse(float ix, float iy);
    void applyTorque(float torque);
    bool isInitialized() const { return B2_IS_NON_NULL(_bodyId); }

    void destroyBody();
};

#endif
