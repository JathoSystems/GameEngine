#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Animation/Keyframe.hpp"
#include "Animation/Converter/LerpConverter.hpp"
#include "GameObjects/GameObject.h"
#include <memory>

#include "../Fixtures/SDLTestFixture.hpp"

TEST_CASE_METHOD(SDLTestFixture, "Keyframe", "[Keyframe]") {

    SECTION("Constructor initializes with transform") {
        auto transform = std::make_unique<Transform>();
        auto keyframe = std::make_unique<Keyframe>(std::move(transform));
        
        REQUIRE(keyframe->getTransform() != nullptr);
    }

    SECTION("getTransform returns correct transform") {
        auto transform = std::make_unique<Transform>();
        Transform* transformPtr = transform.get();
        
        auto keyframe = std::make_unique<Keyframe>(std::move(transform));
        
        REQUIRE(keyframe->getTransform() == transformPtr);
    }

    SECTION("Transition interpolates position correctly") {
        auto gameObject = std::make_unique<GameObject>();
        
        auto startTransform = std::make_unique<Transform>();
        startTransform->getPosition()->setX(0.0f);
        startTransform->getPosition()->setY(0.0f);
        
        auto endTransform = std::make_unique<Transform>();
        endTransform->getPosition()->setX(100.0f);
        endTransform->getPosition()->setY(200.0f);
        
        auto keyframe = std::make_unique<Keyframe>(std::make_unique<Transform>());
        auto converter = std::make_unique<LerpConverter>();
        
        keyframe->transition(
            converter.get(),
            startTransform.get(),
            endTransform.get(),
            gameObject.get(),
            0.5f
        );
        
        Transform* currentTransform = gameObject->getTransform();
        REQUIRE_THAT(currentTransform->getPosition()->getX(), 
                    Catch::Matchers::WithinAbs(50.0f, 0.01f));
        REQUIRE_THAT(currentTransform->getPosition()->getY(), 
                    Catch::Matchers::WithinAbs(100.0f, 0.01f));
    }

    SECTION("Transition interpolates size correctly") {
        auto gameObject = std::make_unique<GameObject>();
        
        auto startTransform = std::make_unique<Transform>();
        startTransform->getSize()->setWidth(10.0f);
        startTransform->getSize()->setHeight(20.0f);
        
        auto endTransform = std::make_unique<Transform>();
        endTransform->getSize()->setWidth(50.0f);
        endTransform->getSize()->setHeight(60.0f);
        
        auto keyframe = std::make_unique<Keyframe>(std::make_unique<Transform>());
        auto converter = std::make_unique<LerpConverter>();
        
        keyframe->transition(
            converter.get(),
            startTransform.get(),
            endTransform.get(),
            gameObject.get(),
            0.5f
        );
        
        Transform* currentTransform = gameObject->getTransform();
        REQUIRE_THAT(currentTransform->getSize()->getWidth(), 
                    Catch::Matchers::WithinAbs(30.0f, 0.01f));
        REQUIRE_THAT(currentTransform->getSize()->getHeight(), 
                    Catch::Matchers::WithinAbs(40.0f, 0.01f));
    }

    SECTION("Transition does not change size if start equals target") {
        auto gameObject = std::make_unique<GameObject>();
        gameObject->getTransform()->getSize()->setWidth(100.0f);
        gameObject->getTransform()->getSize()->setHeight(100.0f);
        
        auto startTransform = std::make_unique<Transform>();
        startTransform->getSize()->setWidth(50.0f);
        startTransform->getSize()->setHeight(50.0f);
        
        auto endTransform = std::make_unique<Transform>();
        endTransform->getSize()->setWidth(50.0f);
        endTransform->getSize()->setHeight(50.0f);
        
        auto keyframe = std::make_unique<Keyframe>(std::make_unique<Transform>());
        auto converter = std::make_unique<LerpConverter>();
        
        keyframe->transition(
            converter.get(),
            startTransform.get(),
            endTransform.get(),
            gameObject.get(),
            0.5f
        );
        
        Transform* currentTransform = gameObject->getTransform();
        // Size should remain unchanged when start equals target
        REQUIRE_THAT(currentTransform->getSize()->getWidth(), 
                    Catch::Matchers::WithinAbs(100.0f, 0.01f));
        REQUIRE_THAT(currentTransform->getSize()->getHeight(), 
                    Catch::Matchers::WithinAbs(100.0f, 0.01f));
    }

    SECTION("Transition interpolates scale correctly") {
        auto gameObject = std::make_unique<GameObject>();
        
        auto startTransform = std::make_unique<Transform>();
        startTransform->getScale()->setScale(1.0f);
        
        auto endTransform = std::make_unique<Transform>();
        endTransform->getScale()->setScale(3.0f);
        
        auto keyframe = std::make_unique<Keyframe>(std::make_unique<Transform>());
        auto converter = std::make_unique<LerpConverter>();
        
        keyframe->transition(
            converter.get(),
            startTransform.get(),
            endTransform.get(),
            gameObject.get(),
            0.5f
        );
        
        Transform* currentTransform = gameObject->getTransform();
        REQUIRE_THAT(currentTransform->getScale()->getScale(), 
                    Catch::Matchers::WithinAbs(2.0f, 0.01f));
    }

    SECTION("Transition does not change scale if start equals target") {
        auto gameObject = std::make_unique<GameObject>();
        gameObject->getTransform()->getScale()->setScale(5.0f);
        
        auto startTransform = std::make_unique<Transform>();
        startTransform->getScale()->setScale(2.0f);
        
        auto endTransform = std::make_unique<Transform>();
        endTransform->getScale()->setScale(2.0f);
        
        auto keyframe = std::make_unique<Keyframe>(std::make_unique<Transform>());
        auto converter = std::make_unique<LerpConverter>();
        
        keyframe->transition(
            converter.get(),
            startTransform.get(),
            endTransform.get(),
            gameObject.get(),
            0.5f
        );
        
        Transform* currentTransform = gameObject->getTransform();
        REQUIRE_THAT(currentTransform->getScale()->getScale(), 
                    Catch::Matchers::WithinAbs(5.0f, 0.01f));
    }
}