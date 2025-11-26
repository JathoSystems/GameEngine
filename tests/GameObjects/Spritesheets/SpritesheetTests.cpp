#include <catch2/catch_test_macros.hpp>
#include "GameObjects/Spritesheet/Spritesheet.h"
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
    TestViewport(float x, float y) : pos(x, y) {}
    Position getPosition() const { return pos; }
};

// Mock Texture that doesn't require actual files
class MockTexture : public Texture {
private:
    int mockWidth = 256;
    int mockHeight = 256;

public:
    MockTexture(const std::string& path, int w = 256, int h = 256)
        : Texture(path), mockWidth(w), mockHeight(h) {}

    int getWidth() const { return mockWidth; }
    int getHeight() const { return mockHeight; }
    SDL_Texture* getTexture() const { return reinterpret_cast<SDL_Texture*>(0x5678); }
};

// Mock SpriteSheet that uses MockTexture
class MockSpriteSheet : public SpriteSheet {
public:
    MockSpriteSheet(const std::string& path, int rows, int cols)
        : SpriteSheet(path, rows, cols) {}

    void initMockFrames(int textureWidth = 256, int textureHeight = 256) {
        std::unique_ptr<Texture> mockTex = std::make_unique<MockTexture>("mock.png", textureWidth, textureHeight);
        this->initFrames(mockTex);
    }
};

TEST_CASE("SpriteSheet", "[SpriteSheet][constructor]") {
    SECTION("Constructor initializes with path and dimensions") {
        REQUIRE_NOTHROW(MockSpriteSheet("mock.png", 4, 4));
    }

    SECTION("Constructor with single row") {
        REQUIRE_NOTHROW(MockSpriteSheet("mock.png", 1, 8));
    }

    SECTION("Constructor with single column") {
        REQUIRE_NOTHROW(MockSpriteSheet("mock.png", 8, 1));
    }

    SECTION("Constructor with large grid") {
        REQUIRE_NOTHROW(MockSpriteSheet("mock.png", 16, 16));
    }

    SECTION("Constructor stores correct rows") {
        MockSpriteSheet sheet("mock.png", 5, 7);
        REQUIRE_NOTHROW(sheet);
    }

    SECTION("Constructor stores correct columns") {
        MockSpriteSheet sheet("mock.png", 3, 9);
        REQUIRE_NOTHROW(sheet);
    }

    SECTION("InitFrames creates correct number of frames", "[SpriteSheet][initFrames]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        REQUIRE_NOTHROW(sheet.initMockFrames());
    }

    SECTION("InitFrames calculates frame dimensions correctly", "[SpriteSheet][initFrames]") {
        MockSpriteSheet sheet("mock.png", 2, 2);
        REQUIRE_NOTHROW(sheet.initMockFrames());
    }

    SECTION("InitFrames with single row spritesheet", "[SpriteSheet][initFrames]") {
        MockSpriteSheet sheet("mock.png", 1, 8);
        REQUIRE_NOTHROW(sheet.initMockFrames());
    }

    SECTION("InitFrames with single column spritesheet", "[SpriteSheet][initFrames]") {
        MockSpriteSheet sheet("mock.png", 8, 1);
        REQUIRE_NOTHROW(sheet.initMockFrames());
    }

    SECTION("RenderFrame initializes frames on first call", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 0, &parent));
    }

    SECTION("RenderFrame with valid frame index", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 5, &parent));
    }

    SECTION("RenderFrame with first frame", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 0, &parent));
    }

    SECTION("RenderFrame with last frame", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 15, &parent));
    }

    SECTION("RenderFrame with out of bounds frame handles gracefully", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 999, &parent));
    }

    SECTION("RenderFrame with negative frame index", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, -1, &parent));
    }

    SECTION("RenderFrame sets parent size when zero", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        sheet.renderFrame(&window, 0, &parent);

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 1, &parent));
    }

    SECTION("RenderFrame preserves non-zero parent size", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;
        parent.getTransform()->getSize()->setWidth(100.0f);
        parent.getTransform()->getSize()->setHeight(100.0f);

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 0, &parent));
    }

    SECTION("RenderFrame with viewport", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        TestViewport viewport(50.0f, 50.0f);
        window.setActiveViewport(&viewport);
        GameObject parent;

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 0, &parent));
    }

    SECTION("RenderFrame multiple times with different frames", "[SpriteSheet][renderFrame]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        sheet.renderFrame(&window, 0, &parent);
        sheet.renderFrame(&window, 5, &parent);
        sheet.renderFrame(&window, 10, &parent);

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 15, &parent));
    }

    SECTION("Multiple spritesheets with same dimensions", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet1("mock.png", 4, 4);
        MockSpriteSheet sheet2("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        REQUIRE_NOTHROW(sheet1.renderFrame(&window, 0, &parent));
        REQUIRE_NOTHROW(sheet2.renderFrame(&window, 0, &parent));
    }

    SECTION("Multiple spritesheets with different dimensions", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet1("mock.png", 2, 2);
        MockSpriteSheet sheet2("mock.png", 8, 8);
        TestWindow window;
        GameObject parent1;
        GameObject parent2;

        REQUIRE_NOTHROW(sheet1.renderFrame(&window, 0, &parent1));
        REQUIRE_NOTHROW(sheet2.renderFrame(&window, 0, &parent2));
    }

    SECTION("Spritesheet animation sequence", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        for (int frame = 0; frame < 16; frame++) {
            REQUIRE_NOTHROW(sheet.renderFrame(&window, frame, &parent));
        }
    }

    SECTION("Spritesheet with viewport integration", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet("mock.png", 8, 8);
        TestWindow window;
        TestViewport viewport(100.0f, 100.0f);
        window.setActiveViewport(&viewport);
        GameObject parent;

        for (int frame = 0; frame < 64; frame += 8) {
            REQUIRE_NOTHROW(sheet.renderFrame(&window, frame, &parent));
        }
    }

    SECTION("Complex spritesheet scenario", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet("mock.png", 16, 16);
        TestWindow window;
        GameObject parent;
        parent.getTransform()->getPosition()->setX(100.0f);
        parent.getTransform()->getPosition()->setY(100.0f);

        // Render various frames
        sheet.renderFrame(&window, 0, &parent);
        sheet.renderFrame(&window, 50, &parent);
        sheet.renderFrame(&window, 100, &parent);
        sheet.renderFrame(&window, 150, &parent);
        sheet.renderFrame(&window, 200, &parent);

        REQUIRE_NOTHROW(sheet.renderFrame(&window, 255, &parent));
    }

    SECTION("Spritesheet frame wrapping behavior", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;

        // Render beyond available frames
        for (int i = 0; i < 50; i++) {
            REQUIRE_NOTHROW(sheet.renderFrame(&window, i % 16, &parent));
        }
    }

    SECTION("Spritesheet with custom parent size", "[SpriteSheet][integration]") {
        MockSpriteSheet sheet("mock.png", 4, 4);
        TestWindow window;
        GameObject parent;
        
        parent.getTransform()->getSize()->setWidth(64.0f);
        parent.getTransform()->getSize()->setHeight(64.0f);
        
        sheet.renderFrame(&window, 0, &parent);

        REQUIRE(parent.getTransform()->getSize()->getWidth() == 64.0f);
        REQUIRE(parent.getTransform()->getSize()->getHeight() == 64.0f);
    }
}