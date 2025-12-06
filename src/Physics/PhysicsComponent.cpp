#include "Physics/PhysicsComponent.h"
#include "Physics/Box2DFacade.h"
#include "GameObjects/GameObject.h"

PhysicsComponent::PhysicsComponent(Box2DFacade* facade)
    : _box2DFacade(facade), _initialized(false) {
    _bodyId = b2_nullBodyId;
}

PhysicsComponent::~PhysicsComponent() {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->destroyBody(_bodyId);
    }
}

void PhysicsComponent::update(float deltaTime) {
    if (!_initialized) {
        initializePhysicsBody();
        _initialized = true;
    }

    if (B2_IS_NON_NULL(_bodyId)) {
        syncTransformFromPhysics();
    }
}

void PhysicsComponent::initializePhysicsBody() {
    if (!_parent || !_collider) return;

    Transform* transform = _parent->getTransform();
    float x = transform->getPosition()->getX();
    float y = transform->getPosition()->getY();

    // If your renderer uses top-left origin, adjust to center for Box2D
    float width = transform->getSize()->getWidth();
    float height = transform->getSize()->getHeight();

    // Adjust position to center (if needed - depends on your coordinate system)
    // Uncomment if your Position is top-left:
    // x += width / 2.0f;
    // y += height / 2.0f;

    float angle = transform->getRotation()->getRotation();

    _bodyId = _box2DFacade->createBody(_parent, _rigidBody.getBodyType(), x, y, angle);

    if (_collider->getType() == ColliderType::BOX) {
        auto* boxCollider = static_cast<BoxCollider*>(_collider.get());
        _box2DFacade->createBoxShape(_bodyId,
                                     boxCollider->getWidth(),
                                     boxCollider->getHeight(),
                                     boxCollider->getOffsetX(),
                                     boxCollider->getOffsetY(),
                                     _rigidBody.getMaterial());
    } else if (_collider->getType() == ColliderType::CIRCLE) {
        auto* circleCollider = static_cast<CircleCollider*>(_collider.get());
        _box2DFacade->createCircleShape(_bodyId,
                                        circleCollider->getRadius(),
                                        circleCollider->getOffsetX(),
                                        circleCollider->getOffsetY(),
                                        _rigidBody.getMaterial());
    }

    _box2DFacade->setGravityScale(_bodyId, _rigidBody.getGravityScale());
    _box2DFacade->setFixedRotation(_bodyId, _rigidBody.isFixedRotation());
}

void PhysicsComponent::syncTransformFromPhysics() {
    if (!_parent) return;

    float x, y;
    _box2DFacade->getPosition(_bodyId, x, y);
    float angle = _box2DFacade->getRotation(_bodyId);

    Transform* transform = _parent->getTransform();

    // If your renderer uses top-left origin, adjust from center
    // Uncomment if needed:
    // float width = transform->getSize()->getWidth();
    // float height = transform->getSize()->getHeight();
    // x -= width / 2.0f;
    // y -= height / 2.0f;

    transform->getPosition()->setX(x);
    transform->getPosition()->setY(y);
    transform->getRotation()->setRotation(angle);
}

void PhysicsComponent::setBodyType(BodyType type) {
    _rigidBody.setBodyType(type);
}

void PhysicsComponent::setCollider(std::unique_ptr<Collider> collider) {
    _collider = std::move(collider);
}

void PhysicsComponent::setMaterial(const Material& material) {
    _rigidBody.setMaterial(material);
}

void PhysicsComponent::setGravityScale(float scale) {
    _rigidBody.setGravityScale(scale);
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->setGravityScale(_bodyId, scale);
    }
}

void PhysicsComponent::setFixedRotation(bool fixed) {
    _rigidBody.setFixedRotation(fixed);
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->setFixedRotation(_bodyId, fixed);
    }
}

void PhysicsComponent::setVelocity(float vx, float vy) {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->setVelocity(_bodyId, vx, vy);
    }
}

void PhysicsComponent::getVelocity(float& vx, float& vy) {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->getVelocity(_bodyId, vx, vy);
    }
}

void PhysicsComponent::applyForce(float fx, float fy) {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->applyForce(_bodyId, fx, fy);
    }
}

void PhysicsComponent::applyImpulse(float ix, float iy) {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->applyImpulse(_bodyId, ix, iy);
    }
}

void PhysicsComponent::applyTorque(float torque) {
    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->applyTorque(_bodyId, torque);
    }
}
