//
// Created by jusra on 10-11-2025.
//

#ifndef GAMEENGINE_SPRITESHEET_H
#define GAMEENGINE_SPRITESHEET_H
#include <vector>

#include "Frame.h"
#include "GameObjects/Texture.h"

class SpriteSheet {
private:
    int _rows;
    int _cols;

    std::unique_ptr<Texture> _texture;
    std::pmr::vector<std::unique_ptr<Frame>> _frames;


public:
    void initFrames(const std::unique_ptr<Texture> & unique);
    SpriteSheet(const std::string &path, int rows, int cols);

    void renderFrame(Window *window, int current_frame, GameObject *parent);
};

#endif //GAMEENGINE_SPRITESHEET_H