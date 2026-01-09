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
    if (!newObject) return;
    
    std::lock_guard<std::mutex> lock(_objectsMutex);
    _pendingObjects.push_back(std::move(newObject));
}

void Scene::processPendingObjects() {
    std::lock_guard<std::mutex> lock(_objectsMutex);
    
    for (auto& newObject : _pendingObjects) {
        if (!newObject) continue;
        
        auto pos = std::lower_bound(_objects.begin(), _objects.end(), newObject,
            [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
                if (!a || !b) return false;
                return a->getLayer() < b->getLayer();
            });

        _objects.insert(pos, std::move(newObject));
    }
    _pendingObjects.clear();
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
    processPendingObjects();
    
    for (auto& obj : _objects) {
        if (!obj || obj->shouldBeDestroyed()) continue;

        obj->update(deltaTime);
    }
    
    _objects.erase(
        std::remove_if(_objects.begin(), _objects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || obj->shouldBeDestroyed();
            }),
        _objects.end()
    );
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
        if (!obj || obj->shouldBeDestroyed()) continue;

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


void Scene::setHUD(std::unique_ptr<HUD> hud) {
    _hud = std::move(hud);
}

HUD* Scene::getHUD() {
    return _hud.get();
}

void Scene::addHUDObject(std::unique_ptr<GameObject> object) {
    if (!_hud) {
        _hud = std::make_unique<HUD>();
    }
    _hud->addObject(std::move(object));
}