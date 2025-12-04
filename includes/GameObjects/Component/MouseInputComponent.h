#ifndef GAMEENGINE_MOUSEINPUTCOMPONENT_H
#define GAMEENGINE_MOUSEINPUTCOMPONENT_H

#include "GameObjects/Component/Component.h"
#include "../../Input/IMouseListener.h"
#include <memory>

#include "GameObjects/Transform/Position.h"

class MouseInputComponent : public Component {
private:
    GameObject *_gameObject;
    IMouseListener *_listener;

public:
    explicit MouseInputComponent(GameObject *gameObject);

    void setListener(IMouseListener *listener);

    void handleMouseMoved(std::unique_ptr<Position> position);

    void handleMouseClicked(MouseButton button);

    void handleMousePressed(MouseButton button);

    void handleMouseReleased(MouseButton button);

    void update(float delta) override;

    void render(const std::unique_ptr<Window> &window) override;
};

#endif
