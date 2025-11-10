//
// Created by kikker234 on 07-11-2025.
//
#include "Scenes/Scene.h"

#include <algorithm>
#include <iostream>

Scene::Scene(std::string name) {
    _name = name;
}

void Scene::addObject(std::unique_ptr<GameObject> newObject) {
    auto pos = std::lower_bound(_objects.begin(), _objects.end(), newObject,
        [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
            return a->getLayer() < b->getLayer();
        });

    _objects.insert(pos, std::move(newObject));
}

std::vector<std::unique_ptr<GameObject>> & Scene::getObjects() {
    return _objects;
}

std::unique_ptr<GameObject> & Scene::getObject(size_t index) {
    return _objects.at(index);
}

const std::string & Scene::getName() const {
    return _name;
}

void Scene::render(const std::unique_ptr<Window> &window) {
    SDL_Renderer* renderer = window->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (const std::unique_ptr<GameObject> & obj : _objects) {
        obj->update();
        obj->render(window);
    }

    SDL_RenderPresent(renderer);
}
