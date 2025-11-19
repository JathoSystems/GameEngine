#ifndef GAMEENGINE_MOUSEINPUTCOMPONENT_H
#define GAMEENGINE_MOUSEINPUTCOMPONENT_H

#include "GameObjects/Component/Component.h"
#include "../../Input/IMouseListener.h"
#include <memory>

class MouseInputComponent : public Component {
private:
    GameObject *_gameObject;
    IMouseListener *_listener;

public:
    explicit MouseInputComponent(GameObject *gameObject);

    void setListener(IMouseListener *listener);

    void handleMouseMoved(MouseButton button);

    void handleMouseClicked(MouseButton button);

    void handleMousePressed(MouseButton button);

    void handleMouseReleased(MouseButton button);

    void update(float delta) override;

    void render(const std::unique_ptr<Window> &window, const Viewport* viewport) override;
};

#endif
