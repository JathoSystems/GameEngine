#pragma once

#include <memory>

#include "Scene.h"
#include "Camera/Camera.h"

class SceneManager {
private:
    std::string _activeScene;
    std::vector<std::unique_ptr<Scene>> _scenes;

public:
    SceneManager();
    void render(const std::unique_ptr<Window> &window, float delta);
    void addScene(std::unique_ptr<Scene> scene);
    void setScene(std::string name);
    std::string getActiveScene();
    Scene* getActiveSceneObj();

};