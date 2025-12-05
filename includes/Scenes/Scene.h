#pragma once

#include <memory>
#include <vector>

#include "GameObjects/GameObject.h"
#include "Scenes/Camera/Camera.h"
#include "SDL/Window.h"
#include "UI/HUD.h"

class Scene {
private:
    std::unique_ptr<Camera> _camera;
    std::unique_ptr<HUD> _hud;
    std::vector<std::unique_ptr<GameObject>> _objects;
    std::string _name;

public:
    Scene(std::string name);
    void setCamera(std::unique_ptr<Camera> camera);
    void addObject(std::unique_ptr<GameObject> object);

    void setHUD(std::unique_ptr<HUD> hud);
    HUD* getHUD();
    void addHUDObject(std::unique_ptr<GameObject> object);

    std::vector<std::unique_ptr<GameObject>>& getObjects() ;
    std::unique_ptr<GameObject>& getObject(size_t index) ;

    const std::string& getName() const;

    void render(const std::unique_ptr<Window> &window, float delta);
};
