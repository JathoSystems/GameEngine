#include "catch2/catch_test_macros.hpp"
#include "Scenes/SceneSystem.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"
#include "SDL/Window.h"

#include <memory>

class DummyScene : public Scene {
public:
    DummyScene(const std::string& name) : Scene(name) {}
    void update(float) {}
    void render(const std::unique_ptr<Window>&, float) {}
};

TEST_CASE("SceneSystem basic functionality", "[SceneSystem]") {
    auto window = std::make_unique<Window>();
    auto sceneManager = std::make_unique<SceneManager>();
    SceneSystem sceneSystem(window, sceneManager.get());

    SECTION("addScene adds a scene") {
        auto scene = std::make_unique<DummyScene>("Scene1");
        sceneSystem.addScene(std::move(scene));
        REQUIRE(sceneSystem.getActiveSceneObj() == nullptr);
    }

    SECTION("setScene activates the correct scene") {
        auto scene = std::make_unique<DummyScene>("Scene1");
        sceneSystem.addScene(std::move(scene));
        sceneSystem.setScene("Scene1");
        REQUIRE(sceneSystem.getActiveSceneObj() != nullptr);
        REQUIRE(sceneSystem.getActiveSceneObj()->getName() == "Scene1");
    }

    SECTION("getActiveSceneObj returns nullptr if no active scene") {
        REQUIRE(sceneSystem.getActiveSceneObj() == nullptr);
    }
}
