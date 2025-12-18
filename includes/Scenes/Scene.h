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
    std::unique_ptr<HUD> _hud;
    std::vector<std::unique_ptr<GameObject>> _objects;
    std::vector<std::unique_ptr<GameObject>> _pendingObjects;
    std::unique_ptr<Camera> _camera;

    void flushPendingObjects();

public:
    explicit Scene(std::string name);

    void setHUD(std::unique_ptr<HUD> hud);
    HUD* getHUD();
    void addHUDObject(std::unique_ptr<GameObject> object);

    void setCamera(std::unique_ptr<Camera> camera);
    void addObject(std::unique_ptr<GameObject> newObject);
    void addObjects(std::vector<std::unique_ptr<GameObject>>& objects);
    void reserveObjects(size_t count);
    std::vector<std::unique_ptr<GameObject>>& getObjects();
    std::unique_ptr<GameObject>& getObject(size_t index);
    const std::string& getName() const;
    void reservePendingObjects(size_t count);


    void update(float deltaTime);
    void render(const std::unique_ptr<Window>& window, float delta);

    virtual void onInitialRender() { };
};

#endif
