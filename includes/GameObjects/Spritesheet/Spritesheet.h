//
// Created by jusra on 10-11-2025.
//

#ifndef GAMEENGINE_SPRITESHEET_H
#define GAMEENGINE_SPRITESHEET_H
#include <vector>

#include "Frame.h"
#include "GameObjects/Transform/Size.h"

class SpriteSheet {
private:
    Size _frameSize;
    std::pmr::vector<Frame> _frames;
};

#endif //GAMEENGINE_SPRITESHEET_H