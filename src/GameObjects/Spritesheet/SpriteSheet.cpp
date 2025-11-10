//
// Created by jusra on 10-11-2025.
//
#include <iostream>

#include "GameObjects/Spritesheet/Spritesheet.h"

SpriteSheet::SpriteSheet(const std::string& path, int rows, int cols) {
    _texture = std::make_unique<Texture>(path);
    _rows = rows;
    _cols = cols;
}

void SpriteSheet::initFrames(const std::unique_ptr<Texture> &unique) {
    int frameWidth = _texture->getWidth() / _cols;
    int frameHeight = _texture->getHeight() / _rows;

    for (int y = 0; y < _rows; y++) {
        for (int x = 0; x < _cols; x++) {
            int srcX = x * frameWidth;
            int srcY = y * frameHeight;
            _frames.push_back(std::make_unique<Frame>(srcX, srcY, frameWidth, frameHeight));
        }
    }
}

void SpriteSheet::renderFrame(Window *window, int current_frame) {
    if (_frames.empty()) {
        _texture->load(window);
        initFrames(_texture);
    }

    try {
        Frame& frame = *_frames.at(current_frame);
        _texture->render(window, &frame);
    } catch (const std::exception e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}
