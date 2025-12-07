#include "catch2/catch_test_macros.hpp"
#include "GameObjects/Component/Behaviour.h"

class TestBehaviour : public Behaviour {
public:
    bool updated = false;

    void onUpdate() override {
        updated = true;
    }
};

TEST_CASE("Behaviour basic behaviour", "[Behaviour]") {

    TestBehaviour behaviour;

    SECTION("update calls onUpdate") {
        REQUIRE(behaviour.updated == false);
        behaviour.update(0.016f);
        REQUIRE(behaviour.updated == true);
    }

    SECTION("render does not crash") {
        REQUIRE_NOTHROW(behaviour.render(nullptr));
    }
}
