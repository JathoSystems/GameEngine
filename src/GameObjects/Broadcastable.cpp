//
// Created by kikker234 on 18-12-2025.
//

#include "GameObjects/Broadcastable.h"

#include "GameObjects/ObjectRegistry.hpp"

Broadcastable::Broadcastable(GameObject* gameObject) : _gameObject(gameObject) {
    _id = ObjectRegistry::getInstance().registerObject(gameObject);
}

Broadcastable::Broadcastable(GameObject* gameObject, int id) : _gameObject(gameObject) {
    _id = id;
    ObjectRegistry::getInstance().insert(gameObject, id);
}

Broadcastable::~Broadcastable() {
    ObjectRegistry& registry = ObjectRegistry::getInstance();
    // Only remove from registry if THIS object is still registered at this ID
    // This prevents removing a NEWER object that has taken the same ID slot
    GameObject* current = registry.getObject(_id);
    if (current == _gameObject) {
        registry.removeObject(_id);
    }
}