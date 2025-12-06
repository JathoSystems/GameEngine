#include "Scenes/SceneManager.h"
#include <chrono>
#include <iostream>

SceneManager::SceneManager() {
}

void SceneManager::update(float deltaTime) {
    Scene* currentScene = nullptr;
    for (const auto& scene : _scenes) {
        if (scene->getName() == _activeScene) {
            currentScene = scene.get();
            break;
        }
    }

    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::render(const std::unique_ptr<Window>& window, float delta) {
    Scene* currentScene = nullptr;
    for (const auto& scene : _scenes) {
        if (scene->getName() == _activeScene) {
            currentScene = scene.get();
            break;
        }
    }

    if (currentScene) {
        currentScene->render(window, delta);
    }
}

void SceneManager::addScene(std::unique_ptr<Scene> scene) {
    _scenes.push_back(std::move(scene));
}

void SceneManager::setScene(std::string name) {
    _activeScene = name;
}

std::string SceneManager::getActiveScene() {
    return _activeScene;
}

Scene* SceneManager::getActiveSceneObj() {
    for (auto& scene : _scenes) {
        if (scene->getName() == _activeScene) {
            return scene.get();
        }
    }
    std::cout << "Failed to fetch scene object: no scene name corresponds to: " << _activeScene << " in _scenes vector\n";
    return nullptr;
}
