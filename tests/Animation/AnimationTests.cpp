#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Animation/Animation.hpp"
#include "Animation/Keyframe.hpp"
#include "Animation/Converter/IConverter.hpp"
#include "GameObjects/GameObject.h"
#include "SDL/Window.h"
#include <memory>

#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Animation", "[Animation]") {

    SECTION("Constructor initializes with correct animation type") {
        auto gameObject = std::make_unique<GameObject>();

        auto anim1 = std::make_unique<Animation>(AnimationType::LERP);
        auto anim2 = std::make_unique<Animation>(AnimationType::EIEO);
        auto anim3 = std::make_unique<Animation>(AnimationType::BOUNCE);
        auto anim4 = std::make_unique<Animation>(AnimationType::ELASTIC);

        Animation* ptr1 = anim1.get();
        Animation* ptr2 = anim2.get();
        Animation* ptr3 = anim3.get();
        Animation* ptr4 = anim4.get();

        gameObject->addComponent(std::move(anim1));
        gameObject->addComponent(std::move(anim2));
        gameObject->addComponent(std::move(anim3));
        gameObject->addComponent(std::move(anim4));

        REQUIRE_NOTHROW(ptr1->update(0.1f));
        REQUIRE_NOTHROW(ptr2->update(0.1f));
        REQUIRE_NOTHROW(ptr3->update(0.1f));
        REQUIRE_NOTHROW(ptr4->update(0.1f));
    }

    SECTION("Default constructor uses EIEO type") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(0.1f));
    }

    SECTION("Adding keyframes stores them correctly") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(0.5f));
    }

    SECTION("Update with empty keyframes doesn't crash") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(0.1f));
        REQUIRE_NOTHROW(animPtr->update(1.0f));
        REQUIRE_NOTHROW(animPtr->update(10.0f));
    }

    SECTION("Render with empty keyframes doesn't crash") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render with single keyframe") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        gameObject->addComponent(std::move(anim));

        animPtr->update(0.5f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render interpolates between keyframes") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(1.0f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render at exact keyframe time") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(1.0f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render after time wraps around") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(5.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(7.0f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render with multiple keyframes") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(0.5f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.5f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(0.75f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render between first and last keyframe wrapping") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(3.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(3.5f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Render at time zero") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Update increments time correctly") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(10.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(1.0f);
        animPtr->update(2.0f);
        animPtr->update(3.0f);

        REQUIRE_NOTHROW(animPtr->update(0.5f));
    }

    SECTION("Time wraps around at max keyframe time") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(5.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(3.0f);
        animPtr->update(3.0f);

        REQUIRE_NOTHROW(animPtr->update(1.0f));
    }

    SECTION("Zero delta time update") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(0.0f));
        REQUIRE_NOTHROW(animPtr->update(0.0f));
    }

    SECTION("Negative time handling") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(-0.1f));
    }

    SECTION("Very large delta time") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        REQUIRE_NOTHROW(animPtr->update(1000.0f));
    }

    SECTION("Keyframes added in non-sequential order") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(0.5f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Duplicate keyframe times") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(1.0f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Animation loops correctly") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(3.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        animPtr->update(1.0f);
        animPtr->update(1.0f);
        animPtr->update(1.0f);
        animPtr->update(1.0f);

        REQUIRE_NOTHROW(gameObject->render(testWindow));
    }

    SECTION("Update and render cycle") {
        auto gameObject = std::make_unique<GameObject>();
        auto anim = std::make_unique<Animation>();
        Animation* animPtr = anim.get();

        animPtr->addKeyframe(0.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(1.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));
        animPtr->addKeyframe(2.0f, std::make_unique<Keyframe>(std::make_unique<Transform>()));

        gameObject->addComponent(std::move(anim));

        for (int i = 0; i < 10; ++i) {
            animPtr->update(0.3f);
            REQUIRE_NOTHROW(gameObject->render(testWindow));
        }
    }
}