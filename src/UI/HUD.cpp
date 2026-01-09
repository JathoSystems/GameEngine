#include "UI/HUD.h"
#include <algorithm>
#include <stdexcept>

void HUD::addObject(std::unique_ptr<GameObject> object) {
    if (!object) return;
    
    auto pos = std::lower_bound(_hudObjects.begin(), _hudObjects.end(), object,
        [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {
            if (!a || !b) return false;
            return a->getLayer() < b->getLayer();
        });
    _hudObjects.insert(pos, std::move(object));
}

void HUD::removeObject(size_t index) {
    if (index >= _hudObjects.size()) {
        throw std::out_of_range("HUD object index out of range");
    }
    _hudObjects.erase(_hudObjects.begin() + static_cast<long>(index));
}

void HUD::clear() {
    _hudObjects.clear();
    _fpsCounter.reset();
}

void HUD::setFPSCounter(std::unique_ptr<FPSCounter> fpsCounter) {
    _fpsCounter = std::move(fpsCounter);
}

FPSCounter* HUD::getFPSCounter() const {
    return _fpsCounter.get();
}

std::vector<std::unique_ptr<GameObject>>& HUD::getObjects() {
    return _hudObjects;
}

GameObject* HUD::getObject(size_t index) {
    if (index >= _hudObjects.size()) {
        return nullptr;
    }
    return _hudObjects[index].get();
}

size_t HUD::getObjectCount() const {
    return _hudObjects.size();
}

void HUD::update(float delta) {
    if (_fpsCounter) {
        _fpsCounter->update(delta);
    }

    for (const auto& obj : _hudObjects) {
        if (!obj || obj->shouldBeDestroyed()) continue;
        obj->update(delta);
    }
}

void HUD::render(const std::unique_ptr<Window>& window) {
    const Viewport* currentViewport = window->getActiveViewport();
    window->setActiveViewport(nullptr);

    if (_fpsCounter) {
        _fpsCounter->render(window);
    }

    for (const auto& obj : _hudObjects) {
        if (!obj || obj->shouldBeDestroyed()) continue;
        obj->render(window);
    }

    window->setActiveViewport(currentViewport);
}
