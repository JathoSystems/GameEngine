#include "Physics/PhysicsComponent.h"

#include <iostream>

#include "Engine/GameEngine.h"
#include "Physics/Box2DFacade.h"
#include "GameObjects/GameObject.h"
#include "Physics/PhysicsSystem.h"
#include "server/GlobalFlags.h"

PhysicsComponent::PhysicsComponent(Box2DFacade* facade)
    : _box2DFacade(facade), _initialized(false) {
    _bodyId = b2_nullBodyId;
    _parent = nullptr;
}

PhysicsComponent::~PhysicsComponent() {
    // Don't destroy body if it's already been cleared
    if (B2_IS_NULL(_bodyId)) {
        return;
    }

    GameEngine *gameEngine = &GameEngine::getInstance();
    PhysicsSystem *physicsSystem = gameEngine->getSystem<PhysicsSystem>();

    if (physicsSystem) {
        physicsSystem->unregisterComponent(this);
    }

    if (B2_IS_NON_NULL(_bodyId) && _box2DFacade) {
        std::cout << "[PhysicsComponent] WARNING: Body not cleaned up properly, destroying in destructor" << std::endl;
        try {
            _box2DFacade->destroyBody(_bodyId);
        } catch (...) {
            std::cout << "[PhysicsComponent] Error destroying body in destructor" << std::endl;
        }
    }

    _bodyId = b2_nullBodyId;
}

void PhysicsComponent::setParent(GameObject *game_object) {
    Component::setParent(game_object);
}

void PhysicsComponent::setPosition(float x, float y) {
    if (_initialized && _box2DFacade && B2_IS_NON_NULL(_bodyId)) {
        _box2DFacade->setPosition(_bodyId, x, y);
    }
}

void PhysicsComponent::update(float deltaTime) {
    // CRITICAL: Don't update during cleanup
    if (GlobalFlags::isLevelCleaning) {
        return;
    }

    if (!_initialized && _parent) {
        initializePhysicsBody();
        _initialized = true;
        return;
    }

    if (B2_IS_NON_NULL(_bodyId)) {
        syncTransformFromPhysics();
    }
}

void PhysicsComponent::initializePhysicsBody() {
    if (!_parent) return;
    if (GlobalFlags::isLevelCleaning) return;

    Transform* transform = _parent->getTransform();
    if (!transform) return;

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
            radius = (width + height) / 4.0f;
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

    if (_rigidBody.getBodyType() == BodyType::DYNAMIC) {
        _box2DFacade->setVelocity(_bodyId, 0.0f, 0.0f);
    }
}

void PhysicsComponent::syncTransformFromPhysics() {
    // CRITICAL: Multiple safety checks
    if (GlobalFlags::isLevelCleaning) {
        return;
    }

    // ADD THIS: Check if physics system is shutting down
    GameEngine *gameEngine = &GameEngine::getInstance();
    PhysicsSystem *physicsSystem = gameEngine->getSystem<PhysicsSystem>();
    if (physicsSystem && physicsSystem->isShuttingDown()) {
        return;
    }

    if (!_parent) {
        return;
    }

    Transform* transform = _parent->getTransform();
    if (!transform) {
        return;
    }

    if (B2_IS_NULL(_bodyId)) {
        return;
    }

    float x, y;
    _box2DFacade->getPosition(_bodyId, x, y);
    float angle = _box2DFacade->getRotation(_bodyId);

    Position* pos = transform->getPosition();
    Rotation* rot = transform->getRotation();

    if (pos) {
        pos->setX(x);
        pos->setY(y);
    }

    if (rot) {
        rot->setRotation(angle);
    }
}

void PhysicsComponent::setBodyType(BodyType type) {
    _rigidBody.setBodyType(type);
}

void PhysicsComponent::setCollider(std::unique_ptr<Collider> collider) {
    _collider = std::move(collider);
    if (!_initialized && _parent && _collider && !GlobalFlags::isLevelCleaning) {
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
    if (B2_IS_NON_NULL(_bodyId) && !GlobalFlags::isLevelCleaning) {
        _box2DFacade->setVelocity(_bodyId, vx, vy);
    }
}

void PhysicsComponent::getVelocity(float& vx, float& vy) {
    if (B2_IS_NON_NULL(_bodyId) && !GlobalFlags::isLevelCleaning) {
        _box2DFacade->getVelocity(_bodyId, vx, vy);
    } else {
        vx = 0.0f;
        vy = 0.0f;
    }
}

void PhysicsComponent::applyForce(float fx, float fy) {
    if (B2_IS_NON_NULL(_bodyId) && !GlobalFlags::isLevelCleaning) {
        _box2DFacade->applyForce(_bodyId, fx, fy);
    }
}

void PhysicsComponent::applyImpulse(float ix, float iy) {
    if (B2_IS_NON_NULL(_bodyId) && !GlobalFlags::isLevelCleaning) {
        _box2DFacade->applyImpulse(_bodyId, ix, iy);
    }
}

void PhysicsComponent::applyTorque(float torque) {
    if (B2_IS_NON_NULL(_bodyId) && !GlobalFlags::isLevelCleaning) {
        _box2DFacade->applyTorque(_bodyId, torque);
    }
}

b2BodyId PhysicsComponent::getBodyId() const {
    return _bodyId;
}

void PhysicsComponent::clearBodyId() {
    _bodyId = b2_nullBodyId;
}
