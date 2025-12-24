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

#include <random>

int ObjectRegistry::generateToken() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 1000);

    _nextTokenId = dist(gen);
    return _nextTokenId;
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
    int limit = 100;
    int counter = 0;
    do {
        key = generateToken();
        counter++;
    } while (counter < limit && _objects.contains(key));

    _objects[key] = obj;
    return key;
}

void ObjectRegistry::removeObject(int key) {
    _objects.erase(key);
}

