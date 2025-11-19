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

std::vector<std::unique_ptr<GameObject>> & Scene::getObjects() {
    return _objects;
}

std::unique_ptr<GameObject> & Scene::getObject(size_t index) {
    return _objects.at(index);
}

const std::string & Scene::getName() const {
    return _name;
}

void Scene::render(const std::unique_ptr<Window> &window, float delta) {
    SDL_Renderer* renderer = window->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Update viewport position based on camera
    Viewport* viewport = nullptr;
    if (_camera) {
        auto cameraPosition = _camera->getPosition();
        viewport = _camera->getViewPort();

        if (viewport) {
            Position viewportPos(
                cameraPosition.getX() - viewport->getSize().getWidth() / 2,
                cameraPosition.getY() - viewport->getSize().getHeight() / 2
            );
            viewport->setPosition(viewportPos);
        }
    }

    for (const std::unique_ptr<GameObject> & obj : _objects) {
        obj->update(delta);
        obj->render(window, viewport);

        auto cameraPosition = _camera.get()->getPosition();
        auto viewPort = _camera->getViewPort()->getPosition();
        std::cout <<  "Scene: " << _name << " Camera Position: "<< cameraPosition.getX()  << ", " << cameraPosition.getY() << "        ";
        std::cout << " Viewport Position: "<< viewPort.getX()  << ", " << viewPort.getY() << std::endl;
    }

    SDL_RenderPresent(renderer);
}