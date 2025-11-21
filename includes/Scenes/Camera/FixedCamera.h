#pragma once
#include "Camera.h"
#include "GameObjects/Transform/Position.h"

class FixedCamera : public Camera {
private:
    Position _position;

public:
    FixedCamera(std::unique_ptr<Viewport> viewport, Position position);
    void move(Position position);
    Position getPosition() const override;
};
