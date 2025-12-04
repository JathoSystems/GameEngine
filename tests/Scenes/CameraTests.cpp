#include <catch2/catch_test_macros.hpp>
#include "Scenes/Camera/Camera.h"
#include "Scenes/Camera/FixedCamera.h"
#include "Scenes/Camera/AttachedCamera.h"
#include "Scenes/Camera/Viewport.h"
#include "GameObjects/GameObject.h"
#include "TestHelper.h"


// =============================================================================
// VIEWPORT TESTS
// =============================================================================

TEST_CASE("Viewport: isInViewPort collision detection", "[Viewport]") {
    Viewport viewport(Size(800, 600), Position(0, 0));

    SECTION("Happy flow - Object fully inside viewport") {
        auto obj = create_game_object(1, Position(100, 100), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Happy flow - Object partially inside viewport (top-left)") {
        auto obj = create_game_object(1, Position(-25, -25), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Happy flow - Object partially inside viewport (bottom-right)") {
        auto obj = create_game_object(1, Position(775, 575), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Happy flow - Object partially inside viewport (top-right)") {
        auto obj = create_game_object(1, Position(775, -25), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Happy flow - Object partially inside viewport (bottom-left)") {
        auto obj = create_game_object(1, Position(-25, 575), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Bad flow - Object completely outside viewport (left)") {
        auto obj = create_game_object(1, Position(-100, 100), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Bad flow - Object completely outside viewport (right)") {
        auto obj = create_game_object(1, Position(900, 100), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Bad flow - Object completely outside viewport (top)") {
        auto obj = create_game_object(1, Position(100, -100), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Bad flow - Object completely outside viewport (bottom)") {
        auto obj = create_game_object(1, Position(100, 700), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Bad flow - Null object pointer") {
        REQUIRE(viewport.isInViewPort(nullptr) == false);
    }

    SECTION("Edge case - Object exactly at viewport boundary (origin)") {
        auto obj = create_game_object(1, Position(0, 0), Size(1, 1));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Edge case - Object exactly at viewport boundary (top-right corner)") {
        auto obj = create_game_object(1, Position(799, 0), Size(1, 1));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Edge case - Object exactly at viewport boundary (bottom-left corner)") {
        auto obj = create_game_object(1, Position(0, 599), Size(1, 1));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Edge case - Object touching viewport edge from outside (right edge)") {
        auto obj = create_game_object(1, Position(800, 300), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Edge case - Object touching viewport edge from outside (bottom edge)") {
        auto obj = create_game_object(1, Position(300, 600), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == false);
    }

    SECTION("Edge case - Very large object overlapping viewport") {
        auto obj = create_game_object(1, Position(-500, -500), Size(2000, 2000));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Edge case - Single pixel object at viewport center") {
        auto obj = create_game_object(1, Position(400, 300), Size(1, 1));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }
}

TEST_CASE("Viewport: isInViewPort with different viewport positions", "[Viewport]") {
    SECTION("Happy flow - Viewport at non-zero position") {
        Viewport viewport(Size(800, 600), Position(100, 100));

        // Object inside the offset viewport
        auto obj1 = create_game_object(1, Position(200, 200), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj1.get()) == true);

        // Object outside but would be inside if viewport was at origin
        auto obj2 = create_game_object(1, Position(50, 50), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj2.get()) == false);
    }

    SECTION("Edge case - Viewport at negative position") {
        Viewport viewport(Size(800, 600), Position(-200, -200));

        auto obj = create_game_object(1, Position(-100, -100), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj.get()) == true);
    }

    SECTION("Edge case - Very large viewport position") {
        Viewport viewport(Size(800, 600), Position(10000, 10000));

        auto obj1 = create_game_object(1, Position(10400, 10300), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj1.get()) == true);

        auto obj2 = create_game_object(1, Position(400, 300), Size(50, 50));
        REQUIRE(viewport.isInViewPort(obj2.get()) == false);
    }
}

// =============================================================================
// FIXED CAMERA TESTS
// =============================================================================

TEST_CASE("FixedCamera: move position updates", "[FixedCamera]") {
    auto viewport = std::make_unique<Viewport>();
    FixedCamera camera(std::move(viewport), Position(0, 0));

    SECTION("Happy flow - Move camera to new position") {
        camera.move(Position(100, 200));
        REQUIRE(camera.getPosition().getX() == 100);
        REQUIRE(camera.getPosition().getY() == 200);
    }

    SECTION("Happy flow - Move camera multiple times") {
        camera.move(Position(100, 100));
        camera.move(Position(200, 200));
        camera.move(Position(300, 300));

        REQUIRE(camera.getPosition().getX() == 300);
        REQUIRE(camera.getPosition().getY() == 300);
    }

    SECTION("Edge case - Move to same position") {
        camera.move(Position(50, 50));
        Position firstPos = camera.getPosition();

        camera.move(Position(50, 50));
        Position secondPos = camera.getPosition();

        REQUIRE(firstPos.getX() == secondPos.getX());
        REQUIRE(firstPos.getY() == secondPos.getY());
    }

    SECTION("Edge case - Move to negative position") {
        camera.move(Position(-500, -300));
        REQUIRE(camera.getPosition().getX() == -500);
        REQUIRE(camera.getPosition().getY() == -300);
    }

    SECTION("Edge case - Move back and forth") {
        camera.move(Position(100, 100));
        camera.move(Position(0, 0));
        camera.move(Position(100, 100));

        REQUIRE(camera.getPosition().getX() == 100);
        REQUIRE(camera.getPosition().getY() == 100);
    }
}

// =============================================================================
// ATTACHED CAMERA TESTS
// =============================================================================

TEST_CASE("AttachedCamera: follows target object", "[AttachedCamera]") {
    auto target = create_game_object(1, Position(500, 400), Size(10, 10));
    GameObject* targetPtr = target.get();

    auto viewport = std::make_unique<Viewport>();
    AttachedCamera camera(std::move(viewport), targetPtr);

    SECTION("Happy flow - Camera follows object position") {
        REQUIRE(camera.getPosition().getX() == 500);
        REQUIRE(camera.getPosition().getY() == 400);

        // Move the target
        targetPtr->getTransform()->getPosition()->setX(600);
        targetPtr->getTransform()->getPosition()->setY(500);

        REQUIRE(camera.getPosition().getX() == 600);
        REQUIRE(camera.getPosition().getY() == 500);
    }

    SECTION("Happy flow - Camera follows object through multiple movements") {
        targetPtr->getTransform()->getPosition()->setX(100);
        targetPtr->getTransform()->getPosition()->setY(100);
        REQUIRE(camera.getPosition().getX() == 100);

        targetPtr->getTransform()->getPosition()->setX(200);
        targetPtr->getTransform()->getPosition()->setY(200);
        REQUIRE(camera.getPosition().getX() == 200);

        targetPtr->getTransform()->getPosition()->setX(300);
        targetPtr->getTransform()->getPosition()->setY(300);
        REQUIRE(camera.getPosition().getX() == 300);
    }

    SECTION("Edge case - Target at origin") {
        targetPtr->getTransform()->getPosition()->setX(0);
        targetPtr->getTransform()->getPosition()->setY(0);
        REQUIRE(camera.getPosition().getX() == 0);
        REQUIRE(camera.getPosition().getY() == 0);
    }

    SECTION("Edge case - Target at negative position") {
        targetPtr->getTransform()->getPosition()->setX(-100);
        targetPtr->getTransform()->getPosition()->setY(-200);
        REQUIRE(camera.getPosition().getX() == -100);
        REQUIRE(camera.getPosition().getY() == -200);
    }
}

TEST_CASE("AttachedCamera: switchAttachedObject behavior", "[AttachedCamera]") {
    auto target1 = create_game_object(1, Position(100, 100), Size(10, 10));
    auto target2 = create_game_object(1, Position(200, 200), Size(10, 10));
    auto target3 = create_game_object(1, Position(300, 300), Size(10, 10));

    GameObject* target1Ptr = target1.get();
    GameObject* target2Ptr = target2.get();
    GameObject* target3Ptr = target3.get();

    auto viewport = std::make_unique<Viewport>();
    AttachedCamera camera(std::move(viewport), target1Ptr);

    SECTION("Happy flow - Switch to new target object") {
        REQUIRE(camera.getFollowing() == target1Ptr);
        REQUIRE(camera.getPosition().getX() == 100);

        camera.switchAttachedObject(target2Ptr);

        REQUIRE(camera.getFollowing() == target2Ptr);
        REQUIRE(camera.getPosition().getX() == 200);
        REQUIRE(camera.getPosition().getY() == 200);
    }

    SECTION("Happy flow - Switch between multiple targets") {
        camera.switchAttachedObject(target2Ptr);
        REQUIRE(camera.getPosition().getX() == 200);

        camera.switchAttachedObject(target3Ptr);
        REQUIRE(camera.getPosition().getX() == 300);

        camera.switchAttachedObject(target1Ptr);
        REQUIRE(camera.getPosition().getX() == 100);
    }

    SECTION("Edge case - Switch to same object") {
        camera.switchAttachedObject(target1Ptr);
        REQUIRE(camera.getFollowing() == target1Ptr);
        REQUIRE(camera.getPosition().getX() == 100);
    }

    SECTION("Bad flow - Switch to null object") {
        camera.switchAttachedObject(nullptr);
        REQUIRE(camera.getFollowing() == nullptr);

        // Calling getPosition() would likely crash or return default position
        // This is a design issue - should be handled in production code
        Position pos = camera.getPosition();
        REQUIRE(pos.getX() == 0);
        REQUIRE(pos.getY() == 0);
    }
}

TEST_CASE("AttachedCamera: tracks moving target in real-time", "[AttachedCamera]") {
    auto player = create_game_object(1, Position(0, 0), Size(32, 32));
    GameObject* playerPtr = player.get();

    auto viewport = std::make_unique<Viewport>();
    AttachedCamera camera(std::move(viewport), playerPtr);

    SECTION("Happy flow - Simulate player movement path") {
        // Simulate game loop with player moving
        std::vector<Position> path = {
            Position(0, 0),
            Position(10, 5),
            Position(20, 10),
            Position(30, 15),
            Position(40, 20)
        };

        for (const auto& pos : path) {
            playerPtr->getTransform()->getPosition()->setX(pos.getX());
            playerPtr->getTransform()->getPosition()->setY(pos.getY());
            Position camPos = camera.getPosition();
            REQUIRE(camPos.getX() == pos.getX());
            REQUIRE(camPos.getY() == pos.getY());
        }
    }

    SECTION("Edge case - Rapid position changes") {
        for (int i = 0; i < 1000; ++i) {
            playerPtr->getTransform()->getPosition()->setX(i);
            playerPtr->getTransform()->getPosition()->setY(i);
            REQUIRE(camera.getPosition().getX() == i);
        }
    }
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

TEST_CASE("Integration: Camera and Viewport working together", "[Integration]") {
    SECTION("Happy flow - Fixed camera with viewport") {
        auto viewport = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        Viewport* vpPtr = viewport.get();
        FixedCamera camera(std::move(viewport), Position(400, 300));

        // Move camera
        camera.move(Position(500, 400));

        // Viewport position should be updated based on camera position
        // (In actual Scene::render, viewport position is calculated as:
        //  cameraPos - viewportSize/2)
        Position expectedViewportPos(
            500 - vpPtr->getSize().getWidth() / 2,
            400 - vpPtr->getSize().getHeight() / 2
        );

        // Simulate what Scene::render does
        vpPtr->setPosition(expectedViewportPos);

        REQUIRE(vpPtr->getPosition().getX() == 100);
        REQUIRE(vpPtr->getPosition().getY() == 100);
    }

    SECTION("Happy flow - Attached camera following player with viewport culling") {
        auto player = create_game_object(1, Position(400, 300), Size(32, 32));
        auto enemy = create_game_object(2, Position(1000, 1000), Size(32, 32));

        GameObject* playerPtr = player.get();
        GameObject* enemyPtr = enemy.get();

        auto viewport = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        Viewport* vpPtr = viewport.get();
        AttachedCamera camera(std::move(viewport), playerPtr);

        // Update viewport position based on camera (simulating Scene::render)
        Position cameraPos = camera.getPosition();
        Position viewportPos(
            cameraPos.getX() - vpPtr->getSize().getWidth() / 2,
            cameraPos.getY() - vpPtr->getSize().getHeight() / 2
        );
        vpPtr->setPosition(viewportPos);

        // Check if objects are in viewport
        REQUIRE(vpPtr->isInViewPort(playerPtr) == true);
        REQUIRE(vpPtr->isInViewPort(enemyPtr) == false);

        // Move player closer to enemy
        playerPtr->getTransform()->getPosition()->setX(900);
        playerPtr->getTransform()->getPosition()->setY(900);

        // Update viewport again
        cameraPos = camera.getPosition();
        viewportPos = Position(
            cameraPos.getX() - vpPtr->getSize().getWidth() / 2,
            cameraPos.getY() - vpPtr->getSize().getHeight() / 2
        );
        vpPtr->setPosition(viewportPos);

        // Now enemy should be visible
        REQUIRE(vpPtr->isInViewPort(playerPtr) == true);
        REQUIRE(vpPtr->isInViewPort(enemyPtr) == true);
    }

    SECTION("Edge case - Switch camera types mid-game") {
        auto player = create_game_object(1, Position(100, 100), Size(32, 32));
        GameObject* playerPtr = player.get();

        // Start with fixed camera
        auto viewport1 = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        FixedCamera fixedCam(std::move(viewport1), Position(400, 300));

        REQUIRE(fixedCam.getPosition().getX() == 400);

        // Switch to attached camera
        auto viewport2 = std::make_unique<Viewport>(Size(800, 600), Position(0, 0));
        AttachedCamera attachedCam(std::move(viewport2), playerPtr);

        REQUIRE(attachedCam.getPosition().getX() == 100);

        // Move player
        playerPtr->getTransform()->getPosition()->setX(500);
        playerPtr->getTransform()->getPosition()->setY(500);
        REQUIRE(attachedCam.getPosition().getX() == 500);
    }
}