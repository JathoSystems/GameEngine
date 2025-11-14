//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_GAMEOBJECT_H
#define GAMEENGINE_GAMEOBJECT_H
#include <memory>
#include <vector>
#include "Component/Component.h"
#include "Transform/Transform.h"

class GameObject {
private:
    int _layer = 1;
    std::unique_ptr<GameObject> _parent;
    std::vector<std::unique_ptr<Component>> _components;
    std::unique_ptr<Transform> _transform = std::make_unique<Transform>();

public:
    void addComponent(std::unique_ptr<Component> component);
    void render(const std::unique_ptr<Window> &window);
    void update(float delta);
    void setLayer(int layer);
    int getLayer();

    Transform *getTransform();
};

#endif //GAMEENGINE_GAMEOBJECT_H
