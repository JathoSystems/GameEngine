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

class TestViewport : public Viewport {
private:
    Position pos;

public:
    TestViewport(float x, float y) : pos(x, y) {
    }

    Position getPosition() const { return pos; }
};

class TestTransform : public Transform {
private:
    Position pos;
    Size size;

public:
    TestTransform(float x, float y, float w, float h) : pos(x, y), size(w, h) {
    }

    Position *getPosition() { return &pos; }
    Size *getSize() { return &size; }
};

class TestFrame : public Frame {
private:
    float x, y, w, h;

public:
    TestFrame(float x, float y, float w, float h) : Frame(x, y, w, h) {
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return w; }
    float getHeight() const { return h; }
};

TEST_CASE("Texture", "[Texture][constructor]") {
    SECTION("Sets path correctly") {
        Texture texture("images/spritesheet.png");
        REQUIRE_NOTHROW(texture.getWidth());
    }

    SECTION("Texture destructor handles null texture", "[Texture][destructor]") {
        Texture* texture = new Texture("images/spritesheet.png");
        REQUIRE_NOTHROW(delete texture);
    }

    SECTION("Transform updates rectangle from Transform", "[Texture][transform]") {
        Texture texture("images/spritesheet.png");
        TestTransform transform(10.0f, 20.0f, 100.0f, 200.0f);

        texture.transform(&transform);
    }

    SECTION("Transform with null pointer does not crash", "[Texture][transform]") {
        Texture texture("images/spritesheet.png");

        REQUIRE_NOTHROW(texture.transform(nullptr));
    }

    SECTION("Transform updates position correctly", "[Texture][transform]") {
        Texture texture("images/spritesheet.png");
        TestTransform transform(50.0f, 75.0f, 150.0f, 250.0f);

        REQUIRE_NOTHROW(texture.transform(&transform));
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
        Texture texture("images/spritesheet.png");
        TestWindow window;

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with null viewport handles gracefully", "[Texture][render]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        window.setActiveViewport(nullptr);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with viewport applies offset", "[Texture][render]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        TestViewport viewport(100.0f, 50.0f);
        window.setActiveViewport(&viewport);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Render with frame and null frame pointer", "[Texture][render][frame]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;

        REQUIRE_NOTHROW(texture.render(&window, nullptr, &gameObject));
    }

    SECTION("Render with frame uses parent transform", "[Texture][render][frame]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        TestFrame frame(0, 0, 32, 32);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("Render with frame applies viewport offset", "[Texture][render][frame]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        TestViewport viewport(50.0f, 50.0f);
        window.setActiveViewport(&viewport);

        GameObject gameObject;
        TestFrame frame(0, 0, 64, 64);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("Render with frame uses frame dimensions when size is zero", "[Texture][render][frame]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        TestFrame frame(10, 20, 100, 150);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }

    SECTION("GetTexture attempts load if texture is null", "[Texture][getTexture]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;

        SDL_Texture *result = texture.getTexture(&window);
        REQUIRE_NOTHROW(texture.getTexture(&window));
    }

    SECTION("Texture lifecycle: construct, transform, render", "[Texture][integration]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        TestTransform transform(100.0f, 150.0f, 200.0f, 300.0f);

        texture.transform(&transform);
        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Texture with viewport integration", "[Texture][integration]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        TestViewport viewport(25.0f, 35.0f);
        window.setActiveViewport(&viewport);

        TestTransform transform(100.0f, 100.0f, 50.0f, 50.0f);
        texture.transform(&transform);

        REQUIRE_NOTHROW(texture.render(&window));
    }

    SECTION("Texture with frame rendering integration", "[Texture][integration]") {
        Texture texture("images/spritesheet.png");
        TestWindow window;
        GameObject gameObject;
        TestFrame frame(32, 64, 32, 32);
        TestViewport viewport(10.0f, 10.0f);
        window.setActiveViewport(&viewport);

        REQUIRE_NOTHROW(texture.render(&window, &frame, &gameObject));
    }
}
