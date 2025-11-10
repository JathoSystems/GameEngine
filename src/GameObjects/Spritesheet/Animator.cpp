//
// Created by jusra on 10-11-2025.
//

#include "GameObjects/Spritesheet/Animator.h"

#include <iostream>

Animator::Animator(std::string path, int rows, int cols) {
    _sheet = std::make_unique<SpriteSheet>(path, rows, cols);
}

void Animator::update(float deltaTime) {
    _accumulator += deltaTime;

    if (_accumulator >= _frameTime) {
        _accumulator -= _frameTime;
        _currentFrame++;

        if (_currentFrame >= _max) {
            _currentFrame = _min;
        }
    }
}


void Animator::render(const std::unique_ptr<Window> &window) {
    _sheet->renderFrame(window.get(), _currentFrame);
}

void Animator::setMin(int min) {
    _currentFrame = min;
    _min = min;
}

void Animator::setMax(int max) {
    _max = max;
}
