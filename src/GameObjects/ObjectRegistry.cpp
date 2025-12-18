//
// Created by jusra on 12-12-2025.
//

#include "../../includes/GameObjects/ObjectRegistry.hpp"

#include <iostream>
#include <random>

#include "Network/NetworkSystem.h"

ObjectRegistry& ObjectRegistry::getInstance() {
    static ObjectRegistry instance;
    return instance;
}

int ObjectRegistry::generateToken() {
    return _nextTokenId++;
}

GameObject* ObjectRegistry::getObject(int key) {
    auto it = _objects.find(key);
    return (it != _objects.end()) ? it->second : nullptr;
}

void ObjectRegistry::insert(GameObject *obj, int key) {
    try {
        _objects[key] = obj;
    } catch (...) {
        std::cerr << "Failed to insert object with key " << key << std::endl;
    }
}

int ObjectRegistry::registerObject(GameObject* obj) {
    int key = generateToken();
    _objects[key] = obj;
    return key;
}

void ObjectRegistry::removeObject(int key) {
    _objects.erase(key);
}

