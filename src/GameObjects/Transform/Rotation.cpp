#include "GameObjects/Transform/Rotation.h"

Rotation::Rotation(int rotation) {
    _rotation = rotation;
}

int Rotation::getRotation() const {
    return _rotation;
}

void Rotation::setRotation(int rotation) {
    _rotation = rotation;
}
