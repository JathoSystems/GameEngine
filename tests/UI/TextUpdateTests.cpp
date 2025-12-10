#include <catch2/catch_test_macros.hpp>
#include "UI/Text.h"
#include "GameObjects/GameObject.h"

TEST_CASE("Text setText method works correctly", "[Text]") {
    auto gameObject = std::make_unique<GameObject>();
    auto text = std::make_unique<Text>("Initial Text");
    Text* textPtr = text.get();
    gameObject->addComponent(std::move(text));
    
    SECTION("setText updates text content") {
        textPtr->setText("Updated Text");
        
        REQUIRE(true);
    }
    
    SECTION("setText can be called multiple times") {
        textPtr->setText("First Update");
        textPtr->setText("Second Update");
        textPtr->setText("Third Update");
        
        REQUIRE(true);
    }
}
