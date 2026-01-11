#include "Scenes/SceneManager.h"
#include <chrono>
#include <iostream>
#include <algorithm>
#include <atomic>

extern std::atomic<uint64_t> sceneGeneration;

SceneManager::SceneManager() {
}

void SceneManager::processRemoveQueue() {
    if (!_removeQueue.empty()) {
        // Increment scene generation to invalidate any pending events for removed scenes
        sceneGeneration.fetch_add(1);
    }
    
    for (const auto& name : _removeQueue) {
        _scenes.erase(
            std::remove_if(_scenes.begin(), _scenes.end(),
                [&name](const std::unique_ptr<Scene>& scene) {
                    return scene && scene->getName() == name;
                }),
            _scenes.end()
        );
        std::cout << "Removed scene: " << name << std::endl;
    }
    _removeQueue.clear();
}

void SceneManager::update(float deltaTime) {
    processRemoveQueue();

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
    // Check if scene with same name already exists
    std::string sceneName = scene->getName();
    for (const auto& existingScene : _scenes) {
        if (existingScene && existingScene->getName() == sceneName) {
            std::cout << "Scene '" << sceneName << "' already exists, not adding duplicate" << std::endl;
            return;
        }
    }
    _scenes.push_back(std::move(scene));
}

void SceneManager::setScene(std::string name) {
    if (_activeScene != name) {
        // Increment scene generation when switching scenes
        sceneGeneration.fetch_add(1);
    }
    _activeScene = name;

    Scene* scene = getActiveSceneObj();

    if (scene)
        scene->onInitialRender();
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

Scene * SceneManager::getScene(const std::string &name) {
    for (const std::unique_ptr<Scene> & scene : _scenes) {
        if (scene->getName() == name)
            return scene.get();
    }

    return nullptr;
}

void SceneManager::removeScene(const std::string& name) {
    if (name == _activeScene) {
        std::cout << "Warning: Cannot remove active scene: " << name << std::endl;
        return;
    }
    std::cout << "Queueing scene for removal: " << name << std::endl;
    _removeQueue.push_back(name);
}
