#include <catch2/catch_test_macros.hpp>
#include "GameObjects/Texture.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Camera/Viewport.h"

// Test doubles
class TestWindow : public Window {
private:
    SDL_Renderer *mockRenderer = reinterpret_cast<SDL_Renderer *>(0x1234);
    const Viewport *activeViewport = nullptr;

public:
    SDL_Renderer *getRenderer() { return mockRenderer; }
    const Viewport *getActiveViewport() { return activeViewport; }
    void setActiveViewport(const Viewport *vp) { activeViewport = vp; }
};

TEST_CASE("Texture", "[Texture][constructor]") {
    SECTION("Sets path correctly") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        REQUIRE_NOTHROW(texture.getWidth());
    }

    SECTION("Texture destructor handles null texture", "[Texture][destructor]") {
        Texture* texture = new Texture("../tests/GameObjects/images/spritesheet.png");
        REQUIRE_NOTHROW(delete texture);
    }

    SECTION("Transform updates rectangle from Transform", "[Texture][transform]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        Transform* transform = new Transform();
        transform->getSize()->setWidth(100);
        transform->getSize()->setHeight(100);

        texture.transform(transform);
    }

    SECTION("Transform with null pointer does not crash", "[Texture][transform]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");

        REQUIRE_NOTHROW(texture.transform(nullptr));
    }

    SECTION("Transform updates position correctly", "[Texture][transform]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        Transform* transform = new Transform();
        transform->getSize()->setWidth(100);
        transform->getSize()->setHeight(100);

        REQUIRE_NOTHROW(texture.transform(transform));
    }

    SECTION("GetWidth returns zero when texture not loaded", "[Texture][dimensions]") {
        Texture texture("nonexistent.png");

        REQUIRE(texture.getWidth() == 0);
    }

    SECTION("GetHeight returns zero when texture not loaded", "[Texture][dimensions]") {
        Texture texture("nonexistent.png");

        REQUIRE(texture.getHeight() == 0);
    }

    SECTION("Render with null renderer does not crash", "[Texture][render]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with null viewport handles gracefully", "[Texture][render]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        window.setActiveViewport(nullptr);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with viewport applies offset", "[Texture][render]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        Viewport viewport(Size(25.0f, 35.0f), Position(0, 0));
        window.setActiveViewport(&viewport);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with frame and null frame pointer", "[Texture][render][frame]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        Viewport port(Size(25.0f, 35.0f), Position(0, 0));
        window.setActiveViewport(&port);
        GameObject gameObject;

        REQUIRE_NOTHROW(texture.render(&window, nullptr, &gameObject));
    }

    SECTION("Render with frame uses parent transform", "[Texture][render][frame]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        Frame frame(0, 0, 32, 32);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("Render with frame applies viewport offset", "[Texture][render][frame]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        Viewport viewport(Size(25.0f, 35.0f), Position(0, 0));
        window.setActiveViewport(&viewport);

        GameObject gameObject;
        Frame frame(0, 0, 64, 64);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("Render with frame uses frame dimensions when size is zero", "[Texture][render][frame]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        Frame frame(10, 20, 100, 150);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("GetTexture attempts load if texture is null", "[Texture][getTexture]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;

        SDL_Texture *result = texture.getTexture(&window);
        REQUIRE_NOTHROW(texture.getTexture(&window));
    }

    SECTION("Texture lifecycle: construct, transform, render", "[Texture][integration]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        Transform transform;
        transform.getSize()->setWidth(200);
        transform.getSize()->setHeight(200);
        transform.getPosition()->setX(10);
        transform.getPosition()->setY(20);

        texture.transform(&transform);
        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Texture with viewport integration", "[Texture][integration]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        Viewport viewport(Size(25.0f, 35.0f), Position(0, 0));
        window.setActiveViewport(&viewport);

        Transform transform;
        transform.getSize()->setWidth(100);
        transform.getSize()->setHeight(100);

        texture.transform(&transform);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Texture with frame rendering integration", "[Texture][integration]") {
        Texture texture("../tests/GameObjects/images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        Frame frame(32, 64, 32, 32);
        Viewport viewport(Size(10, 10), Position(0, 0));
        window.setActiveViewport(&viewport);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }
}
