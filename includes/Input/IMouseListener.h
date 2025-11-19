#ifndef GAMEENGINE_IMOUSELISTENER_H
#define GAMEENGINE_IMOUSELISTENER_H

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

    virtual void onMouseMoved(MouseButton button) = 0;

    virtual void onMouseClicked(MouseButton button) = 0;

    virtual void onMousePressed(MouseButton button) = 0;

    virtual void onMouseReleased(MouseButton button) = 0;
};

#endif
