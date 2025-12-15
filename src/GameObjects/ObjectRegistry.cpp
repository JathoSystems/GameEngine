//
// Created by jusra on 12-12-2025.
//

#include "../../includes/GameObjects/ObjectRegistry.hpp"

ObjectRegistry& ObjectRegistry::getInstance() {
    static ObjectRegistry instance;
    return instance;
}

int ObjectRegistry::generateToken() {
    // Use a simple monotonically increasing counter so that object IDs
    // are deterministic across clients that create objects in the same order.
    // This is important for networking: Move/Jump events send an objectId
    // that must refer to the same logical object on every client.
    static int nextId = 10;
    return nextId++;
}

GameObject* ObjectRegistry::getObject(int key) {
    auto it = _objects.find(key);
    return (it != _objects.end()) ? it->second : nullptr;
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

