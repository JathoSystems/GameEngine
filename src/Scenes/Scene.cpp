#include "Scenes/Scene.h"
#include <algorithm>
#include <iostream>

Scene::Scene(std::string name) {
    _name = name;
}

void Scene::setCamera(std::unique_ptr<Camera> camera) {
    _camera = std::move(camera);
}

void Scene::addObject(std::unique_ptr<GameObject> newObject) {
    auto pos = std::lower_bound(_objects.begin(), _objects.end(), newObject,
        [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
            return a->getLayer() < b->getLayer();
        });

    _objects.insert(pos, std::move(newObject));
}

std::vector<std::unique_ptr<GameObject>>& Scene::getObjects() {
    return _objects;
}

std::unique_ptr<GameObject>& Scene::getObject(size_t index) {
    return _objects.at(index);
}

const std::string& Scene::getName() const {
    return _name;
}

void Scene::update(float deltaTime) {
    for (auto& obj : _objects) {
        obj->update(deltaTime);
    }
}

void Scene::render(const std::unique_ptr<Window>& window, float delta) {
    SDL_Renderer* renderer = window->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    const Viewport* viewport = nullptr;
    if (_camera) {
        auto cameraPosition = _camera->getPosition();
        Viewport* vp = _camera->getViewPort();

        if (vp) {
            Position viewportPos(
                cameraPosition.getX() - vp->getSize().getWidth() / 2,
                cameraPosition.getY() - vp->getSize().getHeight() / 2
            );
            vp->setPosition(viewportPos);
            viewport = vp;
            window->setActiveViewport(viewport);
        }
    }

    for (const std::unique_ptr<GameObject>& obj : _objects) {

        if (!viewport || viewport->isInViewPort(obj.get())) {
            obj->render(window);
        }
    }

    if (_hud) {
        _hud->update(delta);
        _hud->render(window);
    }

    SDL_RenderPresent(renderer);
}
