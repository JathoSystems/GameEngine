#include "GameObjects/Transform/Transform.h"

Transform::Transform() {
    _position = std::make_unique<Position>(0 ,0);
    _rotation = std::make_unique<Rotation>(0);
    _scale = std::make_unique<Scale>(1);
    _size = std::make_unique<Size>(0, 0);
}

Position* Transform::getPosition() {
    return _position.get();
}

Rotation* Transform::getRotation() {
    return _rotation.get();
}

Scale* Transform::getScale() {
    return _scale.get();
}

Size* Transform::getSize() {
    return _size.get();
}
