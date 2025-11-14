//
// Created by kikker234 on 07-11-2025.
//
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
    void render(const std::unique_ptr<Window> &window);
    void addScene(std::unique_ptr<Scene> scene);
    void setScene(std::string name);
    std::string getActiveScene();
    Scene* getActiveSceneObj();

};