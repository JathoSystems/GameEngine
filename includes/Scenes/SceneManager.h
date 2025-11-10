//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_SCENEMANAGER_H
#define GAMEENGINE_SCENEMANAGER_H
#include <memory>
#include <string>

#include "Scene.h"

class SceneManager {
private:
    // FixMe: hardcoded active scene
    std::string _activeScene = "main";
    std::vector<std::unique_ptr<Scene>> _scenes;

public:
    void render(const std::unique_ptr<Window> &window, float delta);

    void addScene(std::unique_ptr<Scene> scene);
};

#endif //GAMEENGINE_SCENEMANAGER_H