//
// Created by jusra on 10-11-2025.
//
#include "GameObjects/Spritesheet/Frame.h"

Frame::Frame(int x, int y, int width, int height)
        : _size(std::make_unique<Size>(width, height)),
          _position(std::make_unique<Position>(0,0)),
          _srcX(x), _srcY(y) {}

Size * Frame::getSize() {
    return _size.get();
}

int Frame::getX() const { return _srcX; }
int Frame::getY() const { return _srcY; }
int Frame::getWidth() const { return _size->getWidth(); }
int Frame::getHeight() const { return _size->getHeight(); }