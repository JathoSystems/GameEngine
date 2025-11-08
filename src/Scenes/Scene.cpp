//
// Created by kikker234 on 07-11-2025.
//
#include "Scenes/Scene.h"

#include <iostream>

Scene::Scene(std::string name) {
    _name = name;
}

void Scene::addObject(std::unique_ptr<GameObject> object) {
    _objects.push_back(std::move(object));
}

const std::vector<std::unique_ptr<GameObject>> & Scene::getObjects() const {
    return _objects;
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
