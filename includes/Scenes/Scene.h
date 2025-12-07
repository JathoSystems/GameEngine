#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <vector>
#include "GameObjects/GameObject.h"
#include "Scenes/Camera/Camera.h"
#include "SDL/Window.h"
#include "UI/HUD.h"

class Scene {
private:
    std::string _name;
    std::vector<std::unique_ptr<GameObject>> _objects;
    std::unique_ptr<Camera> _camera;

public:
    explicit Scene(std::string name);

    void setCamera(std::unique_ptr<Camera> camera);
    void addObject(std::unique_ptr<GameObject> newObject);
    std::vector<std::unique_ptr<GameObject>>& getObjects();
    std::unique_ptr<GameObject>& getObject(size_t index);
    const std::string& getName() const;

    void update(float deltaTime);
    void render(const std::unique_ptr<Window>& window, float delta);
};

#endif
