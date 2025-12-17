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
    static thread_local std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(10, 99);
    return dist(rng);
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
    int key;
    do {
        key = generateToken();
    } while (_objects.contains(key));

    _objects[key] = obj;
    return key;
}

void ObjectRegistry::removeObject(int key) {
    _objects.erase(key);
}

