#include "Scenes/Camera/Viewport.h"
#include "GameObjects/GameObject.h"
#include <iostream>

Viewport::Viewport() : _size{800, 600}, _position{0, 0} {
    std::cout << "Viewport created" << std::endl;
}

Viewport::Viewport(Size size, Position position)
    : _size(size), _position(position) {
    std::cout << "Viewport created with size and position" << std::endl;
}

bool Viewport::isInViewPort(GameObject* object) const {
    if (!object) {
        std::cout << "object is null" << std::endl;
        return false;
    }

    Transform* transform = object->getTransform();

    if (!transform) {
        std::cout << "transform is null" << std::endl;
        return false;
    }

    Position *objPos = transform->getPosition();

    if (!objPos) {
        std::cout << "objPos is null" << std::endl;
        return false;
    }

    Size *objSize = transform->getSize();

    if (!objSize) {
        std::cout << "objSize is null" << std::endl;
        return false;
    }

    return !(objPos->getX() + objSize->getWidth() < _position.getX() ||
             objPos->getX() > _position.getX() + _size.getWidth() ||
             objPos->getY() + objSize->getHeight() < _position.getY() ||
             objPos->getY() > _position.getY() + _size.getHeight());
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