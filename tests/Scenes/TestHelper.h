#pragma once

#include <memory>
#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"
#include "GameObjects/Transform/Position.h"
#include "GameObjects/Transform/Size.h"

// Helper function to create a GameObject with specific layer and transform
inline std::unique_ptr<GameObject> create_game_object(int layer, Position pos = Position(0, 0), Size size = Size(10, 10)) {
    auto obj = std::make_unique<GameObject>();
    obj->setLayer(layer);
    obj->getTransform()->getPosition()->setX(pos.getX());
    obj->getTransform()->getPosition()->setY(pos.getY());
    obj->getTransform()->getSize()->setWidth(size.getWidth());
    obj->getTransform()->getSize()->setHeight(size.getHeight());
    return obj;
}