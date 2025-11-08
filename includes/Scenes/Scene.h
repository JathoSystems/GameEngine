//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H
#include <memory>
#include <vector>

#include "../GameObjects/GameObject.h"

class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> _objects;
    std::string _name;

public:
    Scene(std::string name);
    void addObject(std::unique_ptr<GameObject> object);

    const std::vector<std::unique_ptr<GameObject>>& getObjects() const;

    const std::string& getName() const;

    void render(const std::unique_ptr<Window> &window);
};

#endif //GAMEENGINE_SCENE_H