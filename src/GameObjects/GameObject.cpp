//
// Created by kikker234 on 07-11-2025.
//

#include "GameObjects/GameObject.h"

#include <iostream>
#include <memory>

#include "GameObjects/ObjectRegistry.hpp"

void GameObject::addComponent(std::unique_ptr<Component> component) {
    component->setParent(this);
    _components.emplace_back(std::move(component));
}

void GameObject::render(const std::unique_ptr<Window>& window) {
    for (const std::unique_ptr<Component> & component : _components)
        component->render(window);
}

void GameObject::update(float delta) {
    for (const std::unique_ptr<Component> & component : _components) {
        component->update(delta);
    }
}

void GameObject::setLayer(int layer) {
    _layer = layer;
}

int GameObject::getLayer() {
    return _layer;
}

Transform *GameObject::getTransform() {
    return _transform.get();
}
