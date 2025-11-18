//
// Created by kikker234 on 08-11-2025.
//
#include "GameObjects/Behaviour.h"

void Behaviour::update() {
    onUpdate();
}

void Behaviour::render(const std::unique_ptr<Window> &window, const Viewport *viewport) {
}