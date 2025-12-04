#include "catch2/catch_test_macros.hpp"
#include "AI/transition/LambdaTransition.hpp"
#include <string>

TEST_CASE("LambdaTransition basic behaviour", "[LambdaTransition]") {

    SECTION("shouldTransition returns true when predicate is true") {
        LambdaTransition transition("NextState", []() { return true; });
        REQUIRE(transition.shouldTransition() == true);
        REQUIRE(transition.getTargetState() == "NextState");
    }

    SECTION("shouldTransition returns false when predicate is false") {
        LambdaTransition transition("NextState", []() { return false; });
        REQUIRE(transition.shouldTransition() == false);
        REQUIRE(transition.getTargetState() == "NextState");
    }

    SECTION("predicate can change dynamically") {
        bool flag = false;
        LambdaTransition transition("NextState", [&flag]() { return flag; });

        REQUIRE(transition.shouldTransition() == false);

        flag = true;
        REQUIRE(transition.shouldTransition() == true);
    }
}
