#include "Physics/Box2DFacade.h"
#include "GameObjects/GameObject.h"
#include <iostream>

Box2DFacade::Box2DFacade() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    _worldId = b2CreateWorld(&worldDef);
}

Box2DFacade::~Box2DFacade() {
    if (B2_IS_NON_NULL(_worldId)) {
        b2DestroyWorld(_worldId);
    }
}

void Box2DFacade::init(float gravityX, float gravityY) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravityX, gravityY};

    _worldId = b2CreateWorld(&worldDef);

    if (B2_IS_NULL(_worldId)) {
        std::cerr << "Failed to create Box2D world!" << std::endl;
    }
}

void Box2DFacade::step(float timeStep, int subStepCount) {
    if (B2_IS_NON_NULL(_worldId)) {
        b2World_Step(_worldId, timeStep, subStepCount);
    }
}

void Box2DFacade::setGravity(float x, float y) {
    if (B2_IS_NON_NULL(_worldId)) {
        b2World_SetGravity(_worldId, {x, y});
    }
}

b2BodyId Box2DFacade::createBody(GameObject *userData, BodyType type, float x, float y, float angle) {
    if (B2_IS_NULL(_worldId)) {
        std::cerr << "Cannot create body: world not initialized!" << std::endl;
        return b2_nullBodyId;
    }

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {x, y};
    bodyDef.rotation = b2MakeRot(angle);
    bodyDef.userData = userData;

    switch (type) {
        case BodyType::STATIC:
            bodyDef.type = b2_staticBody;
            break;
        case BodyType::KINEMATIC:
            bodyDef.type = b2_kinematicBody;
            break;
        case BodyType::DYNAMIC:
            bodyDef.type = b2_dynamicBody;
            break;
    }

    b2BodyId bodyId = b2CreateBody(_worldId, &bodyDef);

    if (B2_IS_NULL(bodyId)) {
        std::cerr << "Failed to create body!" << std::endl;
    }

    return bodyId;
}

void Box2DFacade::destroyBody(b2BodyId bodyId) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2DestroyBody(bodyId);
    }
}

void Box2DFacade::createBoxShape(b2BodyId bodyId, float width, float height,
                                 float offsetX, float offsetY, const Material &material) {
    if (B2_IS_NULL(bodyId)) {
        std::cerr << "Cannot create box shape: invalid body!" << std::endl;
        return;
    }

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = material.getDensity();
    shapeDef.friction = material.getFriction();
    shapeDef.restitution = material.getRestitution();

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    b2Polygon box;
    if (offsetX != 0.0f || offsetY != 0.0f) {
        b2Vec2 center = {offsetX, offsetY};
        box = b2MakeOffsetBox(halfWidth, halfHeight, center, 0.0f);
    } else {
        box = b2MakeBox(halfWidth, halfHeight);
    }

    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

    if (B2_IS_NULL(shapeId)) {
        std::cerr << "Failed to create box shape!" << std::endl;
    }
}

void Box2DFacade::createCircleShape(b2BodyId bodyId, float radius,
                                    float offsetX, float offsetY, const Material &material) {
    if (B2_IS_NULL(bodyId)) {
        std::cerr << "Cannot create circle shape: invalid body!" << std::endl;
        return;
    }

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = material.getDensity();
    shapeDef.friction = material.getFriction();
    shapeDef.restitution = material.getRestitution();

    b2Circle circle;
    circle.center = {offsetX, offsetY};
    circle.radius = radius;

    b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);

    if (B2_IS_NULL(shapeId)) {
        std::cerr << "Failed to create circle shape!" << std::endl;
    }
}

void Box2DFacade::getPosition(b2BodyId bodyId, float &x, float &y) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        x = pos.x;
        y = pos.y;
    }
}

void Box2DFacade::setPosition(b2BodyId bodyId, float x, float y) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Rot angle = b2Body_GetRotation(bodyId);
        b2Body_SetTransform(bodyId, {x, y}, angle);
    }
}

float Box2DFacade::getRotation(b2BodyId bodyId) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Rot rot = b2Body_GetRotation(bodyId);
        return b2Rot_GetAngle(rot);
    }
    return 0.0f;
}

void Box2DFacade::setVelocity(b2BodyId bodyId, float vx, float vy) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Body_SetLinearVelocity(bodyId, {vx, vy});
    }
}

void Box2DFacade::getVelocity(b2BodyId bodyId, float &vx, float &vy) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Vec2 vel = b2Body_GetLinearVelocity(bodyId);
        vx = vel.x;
        vy = vel.y;
    }
}

void Box2DFacade::applyForce(b2BodyId bodyId, float fx, float fy) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Vec2 force = {fx, fy};
        b2Vec2 point = b2Body_GetPosition(bodyId);
        b2Body_ApplyForce(bodyId, force, point, true);
    }
}

void Box2DFacade::applyImpulse(b2BodyId bodyId, float ix, float iy) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Vec2 impulse = {ix, iy};
        b2Vec2 point = b2Body_GetPosition(bodyId);
        b2Body_ApplyLinearImpulse(bodyId, impulse, point, true);
    }
}

void Box2DFacade::applyTorque(b2BodyId bodyId, float torque) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Body_ApplyTorque(bodyId, torque, true);
    }
}

void Box2DFacade::setGravityScale(b2BodyId bodyId, float scale) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Body_SetGravityScale(bodyId, scale);
    }
}

void Box2DFacade::setFixedRotation(b2BodyId bodyId, bool fixed) {
    if (B2_IS_NON_NULL(bodyId)) {
        b2Body_SetFixedRotation(bodyId, fixed);
    }
}
