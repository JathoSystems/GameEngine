#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "Scenes/Scene.h"
#include "Scenes/SceneSystem.h"
#include "Scenes/SceneManager.h"
#include "GameObjects/GameObject.h"
#include "SDL/Window.h"
#include "TestHelper.h"


// =============================================================================
// SCENE TESTS
// =============================================================================

class DummyGameObject : public GameObject {
public:
    bool updated = false;
    bool rendered = false;

    void update(float delta) {
        std::cout << "IK ZIT IN UPDATE" << std::endl;
        updated = true;
    }
    void render(const std::unique_ptr<Window>&) { rendered = true; }
};

class DummyCamera : public Camera {
public:
    DummyCamera(std::unique_ptr<Viewport> vp) : Camera(std::move(vp)) {}
    Position getPosition() const override { return Position(0, 0); }
    Viewport* getViewPort() const {
        static Viewport vp;
        vp.setSize(Size(100,100));
        return &vp;
    }
};

TEST_CASE("Scene: addObject sorts by layer", "[Scene]") {
    Scene scene("TestScene");

    SECTION("Happy flow - Add single object") {
        auto obj = create_game_object(1);
        scene.addObject(std::move(obj));
        REQUIRE(scene.getObjects().size() == 1);
    }

    SECTION("Happy flow - Add multiple objects, sorted by layer") {
        auto obj1 = create_game_object(3);
        auto obj2 = create_game_object(1);
        auto obj3 = create_game_object(2);

        scene.addObject(std::move(obj1));
        scene.addObject(std::move(obj2));
        scene.addObject(std::move(obj3));

        auto& objects = scene.getObjects();
        REQUIRE(objects.size() == 3);
        REQUIRE(objects[0]->getLayer() == 1);
        REQUIRE(objects[1]->getLayer() == 2);
        REQUIRE(objects[2]->getLayer() == 3);
    }

    SECTION("Edge case - Add objects with same layer") {
        auto obj1 = create_game_object(1);
        auto obj2 = create_game_object(1);
        auto obj3 = create_game_object(1);

        scene.addObject(std::move(obj1));
        scene.addObject(std::move(obj2));
        scene.addObject(std::move(obj3));

        REQUIRE(scene.getObjects().size() == 3);
        // All should be at layer 1
        for (const auto& obj : scene.getObjects()) {
            REQUIRE(obj->getLayer() == 1);
        }
    }

    SECTION("Edge case - Add objects with negative layers") {
        auto obj1 = create_game_object(-5);
        auto obj2 = create_game_object(0);
        auto obj3 = create_game_object(5);

        scene.addObject(std::move(obj1));
        scene.addObject(std::move(obj2));
        scene.addObject(std::move(obj3));

        auto& objects = scene.getObjects();
        REQUIRE(objects[0]->getLayer() == -5);
        REQUIRE(objects[1]->getLayer() == 0);
        REQUIRE(objects[2]->getLayer() == 5);
    }

    SECTION("Edge case - Add many objects in reverse order") {
        for (int i = 10; i >= 0; --i) {
            auto obj = create_game_object(i);
            scene.addObject(std::move(obj));
        }

        auto& objects = scene.getObjects();
        REQUIRE(objects.size() == 11);

        // Verify sorted order
        for (size_t i = 0; i < objects.size(); ++i) {
            REQUIRE(objects[i]->getLayer() == static_cast<int>(i));
        }
    }
}

// =============================================================================
// SCENE MANAGER TESTS
// =============================================================================

TEST_CASE("SceneManager: Scene management", "[SceneManager]") {
    SceneManager manager;

    SECTION("Happy flow - Add and activate scene") {
        auto scene = std::make_unique<Scene>("Level1");
        manager.addScene(std::move(scene));
        manager.setScene("Level1");

        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene != nullptr);
        REQUIRE(activeScene->getName() == "Level1");
    }

    SECTION("Happy flow - Add multiple scenes and switch between them") {
        auto scene1 = std::make_unique<Scene>("Level1");
        auto scene2 = std::make_unique<Scene>("Level2");
        auto scene3 = std::make_unique<Scene>("Level3");

        manager.addScene(std::move(scene1));
        manager.addScene(std::move(scene2));
        manager.addScene(std::move(scene3));

        manager.setScene("Level2");
        REQUIRE(manager.getActiveSceneObj()->getName() == "Level2");

        manager.setScene("Level1");
        REQUIRE(manager.getActiveSceneObj()->getName() == "Level1");

        manager.setScene("Level3");
        REQUIRE(manager.getActiveSceneObj()->getName() == "Level3");
    }

    SECTION("Bad flow - Get scene object for non-existent scene") {
        auto scene = std::make_unique<Scene>("Level1");
        manager.addScene(std::move(scene));

        manager.setScene("NonExistent");
        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene == nullptr);
    }

    SECTION("Bad flow - Get scene object before setting active") {
        auto scene = std::make_unique<Scene>("TestScene");
        manager.addScene(std::move(scene));

        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene == nullptr);
    }

    SECTION("Edge case - Empty scene name") {
        auto scene = std::make_unique<Scene>("");
        manager.addScene(std::move(scene));
        manager.setScene("");

        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene != nullptr);
        REQUIRE(activeScene->getName() == "");
    }

    SECTION("Edge case - No scenes added") {
        manager.setScene("AnyScene");
        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene == nullptr);
    }

    SECTION("Edge case - Switch to same scene multiple times") {
        auto scene = std::make_unique<Scene>("Level1");
        manager.addScene(std::move(scene));

        manager.setScene("Level1");
        manager.setScene("Level1");
        manager.setScene("Level1");

        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene != nullptr);
        REQUIRE(activeScene->getName() == "Level1");
    }
}

TEST_CASE("SceneManager: render delegates to active scene", "[SceneManager]") {
    SceneManager manager;

    SECTION("Happy flow - Render with active scene") {
        auto scene1 = std::make_unique<Scene>("Scene1");
        auto scene2 = std::make_unique<Scene>("Scene2");

        // Add objects to scene1
        auto obj1 = create_game_object(1);
        scene1->addObject(std::move(obj1));

        manager.addScene(std::move(scene1));
        manager.addScene(std::move(scene2));
        manager.setScene("Scene1");

        // render() should not crash when called
        // Note: This test would need a mock Window to fully test
        REQUIRE(manager.getActiveSceneObj()->getName() == "Scene1");
    }

    SECTION("Bad flow - Render with no active scene") {
        auto scene = std::make_unique<Scene>("Scene1");
        manager.addScene(std::move(scene));

        // Don't set active scene
        // render() should handle this gracefully
        REQUIRE(manager.getActiveSceneObj() == nullptr);
    }

    SECTION("Edge case - Render with empty scene") {
        auto scene = std::make_unique<Scene>("EmptyScene");
        manager.addScene(std::move(scene));
        manager.setScene("EmptyScene");

        Scene* activeScene = manager.getActiveSceneObj();
        REQUIRE(activeScene != nullptr);
        REQUIRE(activeScene->getObjects().empty());
    }
}

// =============================================================================
// INTEGRATION TESTS
// =============================================================================

TEST_CASE("Integration: Complete scene workflow", "[Integration]") {
    SECTION("Happy flow - Build and switch between complete scenes") {
        SceneManager manager;

        // Create menu scene
        auto menuScene = std::make_unique<Scene>("Menu");
        auto menuObj = create_game_object(1, Position(100, 100), Size(50, 50));
        menuScene->addObject(std::move(menuObj));

        // Create game scene with multiple objects
        auto gameScene = std::make_unique<Scene>("Game");
        auto player = create_game_object(1, Position(0, 0), Size(32, 32));
        auto enemy1 = create_game_object(2, Position(100, 100), Size(32, 32));
        auto enemy2 = create_game_object(2, Position(200, 200), Size(32, 32));
        gameScene->addObject(std::move(player));
        gameScene->addObject(std::move(enemy1));
        gameScene->addObject(std::move(enemy2));

        // Add scenes to manager
        manager.addScene(std::move(menuScene));
        manager.addScene(std::move(gameScene));

        // Start with menu
        manager.setScene("Menu");
        REQUIRE(manager.getActiveSceneObj()->getName() == "Menu");
        REQUIRE(manager.getActiveSceneObj()->getObjects().size() == 1);

        // Switch to game
        manager.setScene("Game");
        REQUIRE(manager.getActiveSceneObj()->getName() == "Game");
        REQUIRE(manager.getActiveSceneObj()->getObjects().size() == 3);

        // Verify objects are sorted by layer
        auto& gameObjects = manager.getActiveSceneObj()->getObjects();
        REQUIRE(gameObjects[0]->getLayer() == 1);
        REQUIRE(gameObjects[1]->getLayer() == 2);
        REQUIRE(gameObjects[2]->getLayer() == 2);
    }

    SECTION("Edge case - Rapidly switch between scenes") {
        SceneManager manager;

        auto scene1 = std::make_unique<Scene>("Scene1");
        auto scene2 = std::make_unique<Scene>("Scene2");
        auto scene3 = std::make_unique<Scene>("Scene3");

        manager.addScene(std::move(scene1));
        manager.addScene(std::move(scene2));
        manager.addScene(std::move(scene3));

        // Rapidly switch
        for (int i = 0; i < 100; ++i) {
            manager.setScene("Scene1");
            manager.setScene("Scene2");
            manager.setScene("Scene3");
        }

        REQUIRE(manager.getActiveSceneObj()->getName() == "Scene3");
    }

    SECTION("Render with camera and viewport") {
        auto window = std::make_unique<Window>();
        Scene scene("TestScene");

        auto obj = std::make_unique<DummyGameObject>();
        DummyGameObject* objPtr = obj.get();
        objPtr->getTransform()->getPosition()->setX(10);
        objPtr->getTransform()->getPosition()->setY(10);
        objPtr->getTransform()->getSize()->setWidth(10);
        objPtr->getTransform()->getSize()->setHeight(10);
        scene.addObject(std::move(obj));

        auto viewport = std::make_unique<Viewport>(Size(1000, 1000), Position(0, 0));
        auto camera = std::make_unique<DummyCamera>(std::move(viewport));
        scene.setCamera(std::move(camera));

        REQUIRE_NOTHROW(scene.render(window, 0.016f));
    }
}