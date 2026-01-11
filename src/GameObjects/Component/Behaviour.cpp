#include "GameObjects/Component/Behaviour.h"

void Behaviour::update(float deltaTime) {
    onUpdate();
}

void Behaviour::render(const std::unique_ptr<Window> &window) {
}
