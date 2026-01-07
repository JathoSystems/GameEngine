#include "Physics/PhysicsComponent.h"

#include <iostream>

#include "Engine/GameEngine.h"
#include "Physics/Box2DFacade.h"
#include "GameObjects/GameObject.h"
#include "Physics/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(Box2DFacade* facade)
    : _box2DFacade(facade), _initialized(false) {
    _bodyId = b2_nullBodyId;
    _parent = nullptr;
}

PhysicsComponent::~PhysicsComponent() {
    GameEngine *gameEngine = &GameEngine::getInstance();
    PhysicsSystem *physicsSystem = gameEngine->getSystem<PhysicsSystem>();
    physicsSystem->unregisterComponent(this);

    if (B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->destroyBody(_bodyId);
    }
}

void PhysicsComponent::setParent(GameObject *game_object) {
    Component::setParent(game_object);
    // Initialize physics body immediately when parent is set, using current Transform position
    // Only initialize if we have both parent and collider set
    if (!_initialized && _parent) {
        // Wait for collider to be set if not already set
        // Initialization will happen in update() or when collider is set
    }
}

void PhysicsComponent::setPosition(float x, float y) {
    if (_initialized && _box2DFacade) {
        _box2DFacade->setPosition(_bodyId, x, y);
    }
}

void PhysicsComponent::update(float deltaTime) {
    if (!_initialized && _parent) {
        initializePhysicsBody();
        _initialized = true;
        // Don't sync on first frame to preserve initial Transform position
        return;
    }

    if (B2_IS_NON_NULL(_bodyId)) {
        syncTransformFromPhysics();
    }
}

void PhysicsComponent::initializePhysicsBody() {
    if (!_parent) return;

    Transform* transform = _parent->getTransform();
    float x = transform->getPosition()->getX();
    float y = transform->getPosition()->getY();

    float angle = transform->getRotation()->getRotation();
    _bodyId = _box2DFacade->createBody(_parent, _rigidBody.getBodyType(), x, y, angle);

    if (!_collider) {
        float width = transform->getSize()->getWidth();
        float height = transform->getSize()->getHeight();
        if (width > 0 && height > 0) {
            _collider = std::make_unique<BoxCollider>(width, height);
        } else {
            return;
        }
    }

    if (_collider->getType() == ColliderType::BOX) {
        auto* boxCollider = static_cast<BoxCollider*>(_collider.get());
        float width = boxCollider->getWidth();
        float height = boxCollider->getHeight();

        if (width <= 0 || height <= 0) {
            width = transform->getSize()->getWidth();
            height = transform->getSize()->getHeight();
            boxCollider->setSize(width, height);
        }

        _box2DFacade->createBoxShape(_bodyId,
                                     width,
                                     height,
                                     boxCollider->getOffsetX(),
                                     boxCollider->getOffsetY(),
                                     _rigidBody.getMaterial());
    } else if (_collider->getType() == ColliderType::CIRCLE) {
        auto* circleCollider = static_cast<CircleCollider*>(_collider.get());
        float radius = circleCollider->getRadius();

        if (radius <= 0) {
            float width = transform->getSize()->getWidth();
            float height = transform->getSize()->getHeight();
            radius = (width + height) / 4.0f; // Average half-size
            circleCollider->setRadius(radius);
        }

        _box2DFacade->createCircleShape(_bodyId,
                                        radius,
                                        circleCollider->getOffsetX(),
                                        circleCollider->getOffsetY(),
                                        _rigidBody.getMaterial());
    }

    _box2DFacade->setGravityScale(_bodyId, _rigidBody.getGravityScale());
    _box2DFacade->setFixedRotation(_bodyId, _rigidBody.isFixedRotation());

    // Set initial velocity to zero to prevent falling through ground on first frame
    if (_rigidBody.getBodyType() == BodyType::DYNAMIC) {
        _box2DFacade->setVelocity(_bodyId, 0.0f, 0.0f);
    }
}

void PhysicsComponent::syncTransformFromPhysics() {
    if (!_parent) return;

    float x, y;
    _box2DFacade->getPosition(_bodyId, x, y);
    float angle = _box2DFacade->getRotation(_bodyId);

    Transform* transform = _parent->getTransform();

    transform->getPosition()->setX(x);
    transform->getPosition()->setY(y);
    transform->getRotation()->setRotation(angle);
}


void PhysicsComponent::setBodyType(BodyType type) {
    _rigidBody.setBodyType(type);
}

void PhysicsComponent::setCollider(std::unique_ptr<Collider> collider) {
    _collider = std::move(collider);
    // If parent is already set, initialize physics body now
    if (!_initialized && _parent && _collider) {
        initializePhysicsBody();
        _initialized = true;
    }
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
