#include "Scenes/Camera/Viewport.h"
#include "GameObjects/GameObject.h"
#include <iostream>

Viewport::Viewport() : _size{800, 600}, _position{0, 0} {
}

Viewport::Viewport(Size size, Position position)
    : _size(size), _position(position) {
}

bool Viewport::isInViewPort(GameObject* object) const {
    if (!object) {
        return false;
    }

    Transform* transform = object->getTransform();

    if (!transform) {
        return false;
    }

    Position *objPos = transform->getPosition();

    if (!objPos) {
        return false;
    }

    Size *objSize = transform->getSize();

    if (!objSize) {
        std::cout << "objSize is null" << std::endl;
        return false;
    }

    return !(objPos->getX() + objSize->getWidth() <= _position.getX() ||
             objPos->getX() >= _position.getX() + _size.getWidth() ||
             objPos->getY() + objSize->getHeight() <= _position.getY() ||
             objPos->getY() >= _position.getY() + _size.getHeight());
}

void Viewport::setPosition(Position position) {
    _position = position;
}

void Viewport::setSize(Size size) {
    _size = size;
}

Position Viewport::getPosition() const {
    return _position;
}

Size Viewport::getSize() const {
    return _size;
}