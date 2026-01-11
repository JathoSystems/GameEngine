#include "Scenes/SceneSystem.h"
#include <iostream>

SceneSystem::SceneSystem(const std::unique_ptr<Window>& window, SceneManager* sceneManager)
    : _window(window), _sceneManager(sceneManager) {
}

void SceneSystem::update(float deltaTime) {
    _sceneManager->update(deltaTime);
    _sceneManager->render(_window, deltaTime);
}

void SceneSystem::addScene(std::unique_ptr<Scene> scene) {
    _sceneManager->addScene(std::move(scene));
}

void SceneSystem::setScene(std::string name) {
    if (_sceneManager == nullptr) {
        std::cout << "SceneManager is null, cannot set scene to " << name << std::endl;
        return;
    }
    _sceneManager->setScene(name);
}

void SceneSystem::removeScene(const std::string& name) {
    if (_sceneManager == nullptr) {
        std::cout << "SceneManager is null, cannot remove scene: " << name << std::endl;
        return;
    }
    _sceneManager->removeScene(name);
}

Scene * SceneSystem::getScene(const std::string& name) {
    return _sceneManager->getScene(name);
}

Scene* SceneSystem::getActiveSceneObj() {
    return _sceneManager->getActiveSceneObj();
}
