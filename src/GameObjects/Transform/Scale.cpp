//
// Created by kikker234 on 07-11-2025.
//

#include "GameObjects/Transform/Scale.h"

Scale::Scale(int scale) {
    _scale = scale;
}

int Scale::getScale() const {
    return _scale;
}

void Scale::setScale(int scale) {
    _scale = scale;
}
