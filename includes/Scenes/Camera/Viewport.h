#pragma once
#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Position.h"
#include "GameObjects/Transform/Size.h"

class Viewport {
private:
    Size _size;
    Position _position;

public:
    Viewport();
    Viewport(Size size, Position position);

    bool isInViewPort(GameObject* object) const;

    void setPosition(Position position);
    void setSize(Size size);

    Position getPosition() const;
    Size getSize() const;
};

