#include "Scenes/Camera/Viewport.h"

Viewport::Viewport() : _size{800, 600}, _position{0, 0} {
}

Viewport::Viewport(Size size, Position position)
    : _size(size), _position(position) {
}

bool Viewport::isInViewPort(GameObject* object) const {
    if (!object) return false;

    Position *objPos = object->getTransform()->getPosition();
    Size *objSize = object->getTransform()->getSize();

    // Check if object overlaps with viewport
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