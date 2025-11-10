//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_TRANSFORM_H
#define GAMEENGINE_TRANSFORM_H
#include <memory>

#include "Position.h"
#include "Rotation.h"
#include "Scale.h"
#include "Size.h"

class Transform {
private:
    std::unique_ptr<Position> _position;
    std::unique_ptr<Rotation> _rotation;
    std::unique_ptr<Scale> _scale;
    std::unique_ptr<Size> _size;

public:
    Transform();

    Position* getPosition();
    Rotation* getRotation();
    Scale* getScale();
    Size* getSize();
};

#endif //GAMEENGINE_TRANSFORM_H