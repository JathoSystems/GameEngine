#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Physics/PhysicsComponent.h"
#include "Physics/Box2DFacade.h"
#include "GameObjects/GameObject.h"

TEST_CASE("PhysicsComponent", "[PhysicsComponent]") {

    SECTION("Component with CircleCollider initializes body") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<CircleCollider>(25.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());

        physics.update(0.016f);

        REQUIRE(physics.isInitialized());
    }

    SECTION("Velocity can be set and retrieved") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());
        physics.update(0.016f);

        physics.setVelocity(100.0f, -50.0f);

        float vx, vy;
        physics.getVelocity(vx, vy);

        REQUIRE_THAT(vx, Catch::Matchers::WithinAbs(100.0f, 0.01f));
        REQUIRE_THAT(vy, Catch::Matchers::WithinAbs(-50.0f, 0.01f));
    }

    SECTION("Force application does not crash") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());
        physics.update(0.016f);

        physics.applyForce(200.0f, 0.0f);

        REQUIRE(physics.isInitialized());
    }

    SECTION("Impulse affects velocity") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));
        physics.setMaterial(Material(1.0f, 0.5f, 0.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());
        physics.update(0.016f);

        physics.applyImpulse(0.0f, -100.0f);

        float vx, vy;
        physics.getVelocity(vx, vy);

        REQUIRE(vy != 0.0f);
    }

    SECTION("Torque application does not crash") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());
        physics.update(0.016f);

        physics.applyTorque(10.0f);

        REQUIRE(physics.isInitialized());
    }

    SECTION("Gravity scale can be modified") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());

        physics.setGravityScale(0.5f);
        physics.update(0.016f);

        REQUIRE(physics.isInitialized());
    }

    SECTION("Fixed rotation can be enabled") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);

        PhysicsComponent physics(&facade);
        physics.setBodyType(BodyType::DYNAMIC);
        physics.setCollider(std::make_unique<BoxCollider>(50.0f, 50.0f));

        auto gameObject = std::make_unique<GameObject>();
        physics.setParent(gameObject.get());

        physics.setFixedRotation(true);
        physics.update(0.016f);

        REQUIRE(physics.isInitialized());
    }
}
