#include "Scenes/SceneManager.h"
#include <chrono>

SceneManager::SceneManager() {
    _camera = std::make_unique<Camera>(std::make_unique<Viewport>());
}

void SceneManager::render(const std::unique_ptr<Window>& window) {
    Scene* currentScene = nullptr;
    for (const auto& scene : _scenes) {
        if (scene->getName() == _activeScene) {
            currentScene = scene.get();
            break;
        }
    }

    if (currentScene) {
        currentScene->render(window);
    }
}

void SceneManager::addScene(std::unique_ptr<Scene> scene) {
    _scenes.push_back(std::move(scene));
}

void SceneManager::setScene(std::string name) {
    _activeScene = name;
}