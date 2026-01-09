#pragma once

#include <memory>
#include <vector>
#include "GameObjects/GameObject.h"
#include "FPSCounter.h"
#include "SDL/Window.h"

class HUD {
private:
    std::vector<std::unique_ptr<GameObject>> _hudObjects;
    std::unique_ptr<FPSCounter> _fpsCounter;

public:
    void addObject(std::unique_ptr<GameObject> object);
    void removeObject(size_t index);
    void clear();

    void setFPSCounter(std::unique_ptr<FPSCounter> fpsCounter);

    void removeFpsCounter();

    FPSCounter* getFPSCounter() const;

    std::vector<std::unique_ptr<GameObject>>& getObjects();
    GameObject* getObject(size_t index);
    size_t getObjectCount() const;

    void update(float delta);
    void render(const std::unique_ptr<Window>& window);
};
