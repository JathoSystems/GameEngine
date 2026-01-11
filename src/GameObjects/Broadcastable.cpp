#include "GameObjects/Broadcastable.h"
#include "GameObjects/ObjectRegistry.hpp"

Broadcastable::Broadcastable(GameObject *gameObject) : _gameObject(gameObject) {
    _id = ObjectRegistry::getInstance().registerObject(gameObject);
}

Broadcastable::Broadcastable(GameObject *gameObject, int id) : _gameObject(gameObject) {
    _id = id;
    ObjectRegistry::getInstance().insert(gameObject, id);
}

Broadcastable::~Broadcastable() {
    ObjectRegistry &registry = ObjectRegistry::getInstance();
    GameObject *current = registry.getObject(_id);
    if (current == _gameObject) {
        registry.removeObject(_id);
    }
}
