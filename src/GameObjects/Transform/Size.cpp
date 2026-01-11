#include "GameObjects/Transform/Size.h"

Size::Size(int width, int height) {
    _width = width;
    _height = height;
}

int Size::getWidth() const {
    return _width;
}

void Size::setWidth(int width) {
    _width = width;
}

int Size::getHeight() const {
    return _height;
}

void Size::setHeight(int height) {
    _height = height;
}
