//
// Created by kikker234 on 18-12-2025.
//

#include "GameObjects/Broadcastable.h"

#include "GameObjects/ObjectRegistry.hpp"

Broadcastable::Broadcastable(GameObject* gameObject) {
    _id = ObjectRegistry::getInstance().registerObject(gameObject);
}

Broadcastable::Broadcastable(GameObject* gameObject, int id) {
    _id = id;
    ObjectRegistry::getInstance().insert(gameObject, id);
}

Broadcastable::~Broadcastable() {
    ObjectRegistry& registry = ObjectRegistry::getInstance();
    registry.removeObject(_id);
}