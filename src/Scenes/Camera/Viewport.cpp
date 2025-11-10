#include "Scenes/Camera/Viewport.h"

Viewport::Viewport() : _size{800, 600}, _position{0, 0} {
}

Viewport::Viewport(Size size, Position position)
    : _size(size), _position(position) {
}

bool Viewport::isInViewPort(const GameObject* object) const {
    if (!object) return false;

    Position objPos = object->getPosition();
    Size objSize = object->getSize();

    // Check if object overlaps with viewport
    return !(objPos.x + objSize.width < _position.x ||
             objPos.x > _position.x + _size.width ||
             objPos.y + objSize.height < _position.y ||
             objPos.y > _position.y + _size.height);
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