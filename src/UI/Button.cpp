//
// Created by jusra on 11-11-2025.
//
#include "UI/Button.h"

#include <iostream>

Button::Button(std::unique_ptr<Text> text, std::unique_ptr<Color> backgroundColor) {
    _text = std::move(text);
    _backgroundColor = std::move(backgroundColor);
}

void Button::update(float deltaTime) {
    _text->update(deltaTime);
}

void Button::render(const std::unique_ptr<Window> &window) {
    if (!_text) {
        std::cerr << "[Button] _text is nullptr!\n";
        return;
    }
    if (!window) {
        std::cerr << "[Button] window is nullptr!\n";
        return;
    }

    std::cerr << "Attempt to render text" <<std::endl;
    _text->setParent(_parent);
    _text->render(window);
}
