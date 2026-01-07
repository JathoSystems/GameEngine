#ifndef GAMEENGINE_SCENESYSTEM_H
#define GAMEENGINE_SCENESYSTEM_H
#include <memory>

#include "Scene.h"
#include "SceneManager.h"
#include "Engine/ISystem.h"

class SceneSystem : public ISystem {
private:
    SceneManager* _sceneManager;
    const std::unique_ptr<Window> & _window;

public:
    ~SceneSystem() = default;

    SceneSystem(const std::unique_ptr<Window> &window, SceneManager* sceneManager);  // Add parameter

    void update(float deltaTime) override;
    void addScene(std::unique_ptr<Scene> scene);
    void setScene(std::string name);
    Scene* getScene(const std::string &name);
    Scene* getActiveSceneObj();
};

#endif //GAMEENGINE_SCENESYSTEM_H