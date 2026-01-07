#ifndef BOX2DFACADE_H
#define BOX2DFACADE_H

#include <box2d/box2d.h>
#include <functional>
#include "Physics/BodyType.h"
#include "Physics/Material.h"

class GameObject;

class Box2DFacade {
private:
    b2WorldId _worldId;

public:
    Box2DFacade();
    ~Box2DFacade();

    void init(float gravityX, float gravityY);
    void step(float timeStep, int subStepCount);
    void setGravity(float x, float y);

    b2BodyId createBody(GameObject* userData, BodyType type, float x, float y, float angle);
    void destroyBody(b2BodyId bodyId);

    void createBoxShape(b2BodyId bodyId, float width, float height,
                       float offsetX, float offsetY, const Material& material);
    void createCircleShape(b2BodyId bodyId, float radius,
                          float offsetX, float offsetY, const Material& material);

    void getPosition(b2BodyId bodyId, float& x, float& y);
    void setPosition(b2BodyId bodyId, float x, float y);
    float getRotation(b2BodyId bodyId);
    void setVelocity(b2BodyId bodyId, float vx, float vy);
    void getVelocity(b2BodyId bodyId, float& vx, float& vy);

    void applyForce(b2BodyId bodyId, float fx, float fy);
    void applyImpulse(b2BodyId bodyId, float ix, float iy);
    void applyTorque(b2BodyId bodyId, float torque);

    void setGravityScale(b2BodyId bodyId, float scale);
    void setFixedRotation(b2BodyId bodyId, bool fixed);

    b2WorldId getWorldId() const { return _worldId; }
};

#endif
