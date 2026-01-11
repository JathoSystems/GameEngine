#include "GameObjects/Spritesheet/Animator.h"

Animator::Animator(std::string path, int rows, int cols) {
    _sheet = std::make_unique<SpriteSheet>(path, rows, cols);
    _min = 0;
    _max = rows * cols;
    _rows = rows;
    _cols = cols;
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
    _sheet->renderFrame(window.get(), _currentFrame, _parent);
}

void Animator::setMin(int min) {
    _currentFrame = min;
    _min = min;
}

void Animator::setMax(int max) {
    _max = max;
}

int Animator::getMin() const {
    return _min;
}

int Animator::getMax() const {
    return _max;
}

int Animator::getTotalFrames() const {
    return _rows * _cols;
}
