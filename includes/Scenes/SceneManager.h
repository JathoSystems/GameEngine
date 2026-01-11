#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <string>
#include <vector>
#include "Scenes/Scene.h"
#include "SDL/Window.h"

class SceneManager {
private:
    std::vector<std::unique_ptr<Scene>> _scenes;
    std::string _activeScene;

public:
    SceneManager();

    void update(float deltaTime);
    void render(const std::unique_ptr<Window>& window, float delta);
    void addScene(std::unique_ptr<Scene> scene);
    void setScene(std::string name);
    std::string getActiveScene();
    Scene* getActiveSceneObj();

    Scene * getScene(const std::string & name);

    void removeScene(const std::string & name);
};

#endif
