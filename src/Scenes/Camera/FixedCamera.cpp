#include "Scenes/Camera/FixedCamera.h"

FixedCamera::FixedCamera(std::unique_ptr<Viewport> viewport, Position position)
    : Camera(std::move(viewport)), _position(position) {
}

void FixedCamera::move(Position position) {
    _position = position;
}

Position FixedCamera::getPosition() const {
    return _position;
}