#include "catch2/catch_test_macros.hpp"

#include "GameObjects/GameObject.h"
#include "GameObjects/Component/SpriteRenderer.h"
#include "GameObjects/Transform/Transform.h"
#include "SDL/Window.h"

static const std::string TEST_TEXTURE = "../tests/GameObjects/images/spritesheet.png";

TEST_CASE("SpriteRenderer", "[SpriteRenderer][GameObject]") {

    GameObject obj;

    REQUIRE(obj.getTransform() != nullptr);

    auto renderer = std::make_unique<SpriteRenderer>(TEST_TEXTURE);
    SpriteRenderer* rendererPtr = renderer.get();

    obj.addComponent(std::move(renderer));
    REQUIRE(rendererPtr != nullptr);

    SECTION("Size stays the same when configured") {

        auto size = obj.getTransform()->getSize();

        int originalW = 123;
        int originalH = 456;
        size->setWidth(originalW);
        size->setHeight(originalH);

        obj.update(0.016f);

        REQUIRE(size->getWidth()  == originalW);
        REQUIRE(size->getHeight() == originalH);
    }

    SECTION("update() doesn't crash") {

        auto size = obj.getTransform()->getSize();
        size->setWidth(100);
        size->setHeight(100);

        REQUIRE_NOTHROW(obj.update(0.016f));
    }
}
