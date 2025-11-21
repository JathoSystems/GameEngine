//
// Created by kikker234 on 08-11-2025.
//
#include "GameObjects/Component/Behaviour.h"

void Behaviour::update(float deltaTime) {
    onUpdate();
}

void Behaviour::render(const std::unique_ptr<Window> &window) {
}