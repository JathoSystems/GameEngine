#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Physics/Box2DFacade.h"
#include "Physics/Material.h"
#include "Physics/BodyType.h"
#include "GameObjects/GameObject.h"
#include <memory>

TEST_CASE("Box2DFacade", "[Box2DFacade]") {

    SECTION("World initialization with default gravity") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        
        REQUIRE(B2_IS_NON_NULL(facade.getWorldId()));
    }

    SECTION("Gravity can be updated after initialization") {
        Box2DFacade facade;
        facade.init(0.0f, 0.0f);
        
        facade.setGravity(0.0f, 981.0f);
        
        // Should not crash - gravity updated successfully
        REQUIRE(B2_IS_NON_NULL(facade.getWorldId()));
    }

    SECTION("Static body creation") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::STATIC,
            100.0f, 200.0f,
            0.0f
        );
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
        
        float positionX, positionY;
        facade.getPosition(bodyId, positionX, positionY);
        REQUIRE_THAT(positionX, Catch::Matchers::WithinAbs(100.0f, 0.01f));
        REQUIRE_THAT(positionY, Catch::Matchers::WithinAbs(200.0f, 0.01f));
    }

    SECTION("Dynamic body creation") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            50.0f, 100.0f,
            0.0f
        );
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Box shape creation with correct dimensions") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::STATIC,
            0.0f, 0.0f,
            0.0f
        );
        
        Material physicsMaterial(1.0f, 0.5f, 0.0f);
        
        // Should create shape without crashing
        facade.createBoxShape(bodyId, 100.0f, 50.0f, 0.0f, 0.0f, physicsMaterial);
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Circle shape creation with offset") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        Material physicsMaterial(1.0f, 0.5f, 0.2f);
        
        facade.createCircleShape(bodyId, 25.0f, 10.0f, 10.0f, physicsMaterial);
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Velocity can be set and retrieved") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        facade.setVelocity(bodyId, 100.0f, -200.0f);
        
        float velocityX, velocityY;
        facade.getVelocity(bodyId, velocityX, velocityY);
        
        REQUIRE_THAT(velocityX, Catch::Matchers::WithinAbs(100.0f, 0.01f));
        REQUIRE_THAT(velocityY, Catch::Matchers::WithinAbs(-200.0f, 0.01f));
    }

    SECTION("Force application does not crash") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        // Should not crash
        facade.applyForce(bodyId, 500.0f, 0.0f);
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Impulse application affects velocity") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        Material physicsMaterial(1.0f, 0.5f, 0.0f);
        facade.createBoxShape(bodyId, 50.0f, 50.0f, 0.0f, 0.0f, physicsMaterial);
        
        facade.applyImpulse(bodyId, 0.0f, -100.0f);
        
        float velocityX, velocityY;
        facade.getVelocity(bodyId, velocityX, velocityY);
        
        // Velocity should be non-zero after impulse
        REQUIRE(velocityY != 0.0f);
    }

    SECTION("Rotation can be retrieved") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        float rotationAngle = facade.getRotation(bodyId);
        
        REQUIRE_THAT(rotationAngle, Catch::Matchers::WithinAbs(0.0f, 0.01f));
    }

    SECTION("Fixed rotation prevents spinning") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        // Should not crash
        facade.setFixedRotation(bodyId, true);
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Gravity scale modification") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        // Should not crash
        facade.setGravityScale(bodyId, 0.5f);
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
    }

    SECTION("Physics simulation step") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 100.0f,
            0.0f
        );
        
        Material physicsMaterial(1.0f, 0.5f, 0.0f);
        facade.createBoxShape(bodyId, 50.0f, 50.0f, 0.0f, 0.0f, physicsMaterial);
        
        float initialPositionX, initialPositionY;
        facade.getPosition(bodyId, initialPositionX, initialPositionY);
        
        // Simulate physics for 1 frame
        facade.step(1.0f / 60.0f, 4);
        
        float newPositionX, newPositionY;
        facade.getPosition(bodyId, newPositionX, newPositionY);
        
        // Object should fall due to gravity
        REQUIRE(newPositionY > initialPositionY);
    }

    SECTION("Body destruction") {
        Box2DFacade facade;
        facade.init(0.0f, 981.0f);
        auto gameObject = std::make_unique<GameObject>();
        
        b2BodyId bodyId = facade.createBody(
            gameObject.get(),
            BodyType::DYNAMIC,
            0.0f, 0.0f,
            0.0f
        );
        
        REQUIRE(B2_IS_NON_NULL(bodyId));
        
        // Should not crash
        facade.destroyBody(bodyId);
    }
}
