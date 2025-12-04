#ifndef GAMEENGINE_IMOUSELISTENER_H
#define GAMEENGINE_IMOUSELISTENER_H

#include "GameObjects/Transform/Position.h"
#include <memory>

enum class MouseButton {
    LEFT,
    MIDDLE,
    RIGHT,
    X1,
    X2
};

class IMouseListener {
public:
    virtual ~IMouseListener() = default;

    virtual void onMouseMoved(std::unique_ptr<Position> position) = 0;

    virtual void onMouseClicked(MouseButton button) = 0;

    virtual void onMousePressed(MouseButton button) = 0;

    virtual void onMouseReleased(MouseButton button) = 0;
};

#endif
