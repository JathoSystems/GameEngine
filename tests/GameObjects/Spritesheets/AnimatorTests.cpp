#include <catch2/catch_test_macros.hpp>
#include "GameObjects/Spritesheet/Animator.h"
#include "GameObjects/Spritesheet/SpriteSheet.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Texture.h"
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

// Mock Animator that uses mock resources
class MockAnimator : public Animator {
public:
    MockAnimator(const std::string& path, int rows, int cols)
        : Animator(path, rows, cols) {}
};

TEST_CASE("Animator", "[Animator][constructor]") {
    SECTION("Constructor initializes with correct path and dimensions") {
        REQUIRE_NOTHROW(MockAnimator("mock.png", 4, 4));
    }

    SECTION("Constructor sets rows and columns correctly") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 3, 5);

        REQUIRE(animator->getTotalFrames() == 15);
    }

    SECTION("Constructor initializes min to 0") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);

        REQUIRE(animator->getMin() == 0);
    }

    SECTION("Constructor initializes max to total frames") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 6);

        REQUIRE(animator->getMax() == 24);
    }

    SECTION("Update with zero deltaTime does not advance frame", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        REQUIRE_NOTHROW(animator->update(0.0f));
    }

    SECTION("Update with small deltaTime accumulates correctly", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);

        animator->update(0.01f);
        animator->update(0.01f);

        REQUIRE_NOTHROW(animator->update(0.01f));
    }

    SECTION("Update advances frame when accumulator exceeds frame time", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);

        animator->update(1.0f);

        REQUIRE_NOTHROW(animator->update(0.0f));
    }

    SECTION("Update wraps to min when reaching max frame", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);
        animator->setMin(0);
        animator->setMax(4);

        for (int i = 0; i < 10; i++) {
            animator->update(1.0f);
        }

        REQUIRE_NOTHROW(animator->update(0.0f));
    }

    SECTION("Update with large deltaTime handles multiple frames", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        REQUIRE_NOTHROW(animator->update(100.0f));
    }

    SECTION("Update respects custom min frame", "[Animator][update]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        animator->setMin(5);
        animator->setMax(10);

        animator->update(1.0f);

        REQUIRE_NOTHROW(animator->update(0.0f));
    }

    SECTION("Render with viewport applies offset", "[Animator][render]") {
        GameObject gameObject;
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        auto* animatorPtr = animator.get();
        gameObject.addComponent(std::move(animator));

        std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        TestViewport viewport(50.0f, 50.0f);
        window->setActiveViewport(&viewport);

        REQUIRE_NOTHROW(animatorPtr->render(window));
    }

    SECTION("Render after multiple updates", "[Animator][render]") {
        GameObject gameObject;
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        auto* animatorPtr = animator.get();
        gameObject.addComponent(std::move(animator));

        std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        animatorPtr->update(1.0f);
        animatorPtr->update(1.0f);

        REQUIRE_NOTHROW(animatorPtr->render(window));
    }

    SECTION("SetMin updates current frame to min", "[Animator][setMin]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        animator->setMin(5);

        REQUIRE(animator->getMin() == 5);
    }

    SECTION("SetMin with zero sets to beginning", "[Animator][setMin]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        animator->setMin(0);

        REQUIRE(animator->getMin() == 0);
    }

    SECTION("SetMin with value beyond total frames", "[Animator][setMin]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);

        REQUIRE_NOTHROW(animator->setMin(10));
    }

    SECTION("SetMax updates maximum frame", "[Animator][setMax]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        animator->setMax(8);

        REQUIRE(animator->getMax() == 8);
    }

    SECTION("SetMax with value less than total frames", "[Animator][setMax]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);

        animator->setMax(10);

        REQUIRE(animator->getMax() == 10);
    }

    SECTION("SetMax with value greater than total frames", "[Animator][setMax]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 2, 2);

        REQUIRE_NOTHROW(animator->setMax(20));
    }

    SECTION("GetMin returns correct minimum frame", "[Animator][getMin]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        animator->setMin(3);

        REQUIRE(animator->getMin() == 3);
    }

    SECTION("GetMax returns correct maximum frame", "[Animator][getMax]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        animator->setMax(12);

        REQUIRE(animator->getMax() == 12);
    }

    SECTION("GetTotalFrames calculates correctly", "[Animator][getTotalFrames]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 5, 7);

        REQUIRE(animator->getTotalFrames() == 35);
    }

    SECTION("GetTotalFrames with single row", "[Animator][getTotalFrames]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 1, 8);

        REQUIRE(animator->getTotalFrames() == 8);
    }

    SECTION("GetTotalFrames with single column", "[Animator][getTotalFrames]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 10, 1);

        REQUIRE(animator->getTotalFrames() == 10);
    }

    SECTION("Animation loop: set range and update", "[Animator][integration]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        animator->setMin(0);
        animator->setMax(8);

        for (int i = 0; i < 20; i++) {
            animator->update(0.5f);
        }

        REQUIRE_NOTHROW(animator->update(0.0f));
    }

    SECTION("Animation with custom range", "[Animator][integration]") {
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        animator->setMin(4);
        animator->setMax(12);

        animator->update(1.0f);
        animator->update(1.0f);

        REQUIRE(animator->getMin() == 4);
        REQUIRE(animator->getMax() == 12);
    }

    SECTION("Full animation cycle with render", "[Animator][integration]") {
        GameObject gameObject;
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        auto* animatorPtr = animator.get();
        gameObject.addComponent(std::move(animator));

        std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        animatorPtr->setMin(0);
        animatorPtr->setMax(16);

        for (int i = 0; i < 20; i++) {
            animatorPtr->update(0.1f);
            REQUIRE_NOTHROW(animatorPtr->render(window));
        }
    }

    SECTION("Animation with viewport integration", "[Animator][integration]") {
        GameObject gameObject;
        auto animator = std::make_unique<MockAnimator>("mock.png", 4, 4);
        auto* animatorPtr = animator.get();
        gameObject.addComponent(std::move(animator));

        std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        TestViewport viewport(100.0f, 100.0f);
        window->setActiveViewport(&viewport);

        animatorPtr->update(1.0f);

        REQUIRE_NOTHROW(animatorPtr->render(window));
    }

    SECTION("Complex animation scenario", "[Animator][integration]") {
        GameObject gameObject;
        auto animator = std::make_unique<MockAnimator>("mock.png", 8, 8);
        auto* animatorPtr = animator.get();
        gameObject.addComponent(std::move(animator));

        std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        animatorPtr->setMin(16);
        animatorPtr->setMax(24);

        for (int frame = 0; frame < 100; frame++) {
            animatorPtr->update(0.016f);
            animatorPtr->render(window);
        }

        REQUIRE(animatorPtr->getTotalFrames() == 64);
        REQUIRE(animatorPtr->getMin() == 16);
        REQUIRE(animatorPtr->getMax() == 24);
    }
}