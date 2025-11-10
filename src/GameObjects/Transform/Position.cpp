//
// Created by kikker234 on 07-11-2025.
//

#include "GameObjects/Transform/Position.h"

Position::Position(int x, int y) {
    _x = x;
    _y = y;
}

int Position::getX() const {
    return _x;
}

void Position::setX(int x) {
    _x = x;
}

int Position::getY() const {
    return _y;
}

void Position::setY(int y) {
    _y = y;
}
