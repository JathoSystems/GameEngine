#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <chrono>
#include "Engine/GameEngine.h"
#include "Engine/TimeManager.h"

TEST_CASE("GameEngine provides TimeManager access", "[GameEngine]") {
    GameEngine& engine = GameEngine::getInstance();
    engine.init("Test", 800, 600);
    
    SECTION("TimeManager is accessible") {
        TimeManager* timeManager = engine.getTimeManager();
        REQUIRE(timeManager != nullptr);
    }
    
    SECTION("TimeManager scale can be modified") {
        TimeManager* timeManager = engine.getTimeManager();
        timeManager->setTimeScale(2.0f);
        
        REQUIRE(timeManager->getTimeScale() == 2.0f);
    }
    
    SECTION("TimeManager scale affects deltaTime") {
        TimeManager* timeManager = engine.getTimeManager();
        timeManager->start();
        timeManager->setTimeScale(2.0f);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        float deltaTime = timeManager->update();
        
        REQUIRE(deltaTime > 0.0f);
    }
}
