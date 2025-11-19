#ifndef GAMEENGINE_COMPONENT_H
#define GAMEENGINE_COMPONENT_H

#include "SDL/Window.h"
#include <memory>

class GameObject;
class Viewport;
class Window;

class Component {
protected:
    // Geen ownership, alleen kennis van. Daarom een rawpointer want component mag deze niet verwijderen
    GameObject* _parent;

public:
    virtual ~Component() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(const std::unique_ptr<Window> &window, const Viewport* viewport) = 0;

    void setParent(GameObject *game_object) {
        _parent = game_object;
    }
};

#endif //GAMEENGINE_COMPONENT_H