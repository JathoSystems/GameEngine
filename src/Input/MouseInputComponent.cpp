#include "GameObjects/Component/MouseInputComponent.h"

MouseInputComponent::MouseInputComponent(GameObject *gameObject)
    : _gameObject(gameObject), _listener(nullptr) {
}

void MouseInputComponent::setListener(IMouseListener *listener) {
    _listener = listener;
}

void MouseInputComponent::handleMouseMoved(MouseButton button) {
    if (_listener) {
        _listener->onMouseMoved(button);
    }
}

void MouseInputComponent::handleMouseClicked(MouseButton button) {
    if (_listener) {
        _listener->onMouseClicked(button);
    }
}

void MouseInputComponent::handleMousePressed(MouseButton button) {
    if (_listener) {
        _listener->onMousePressed(button);
    }
}

void MouseInputComponent::handleMouseReleased(MouseButton button) {
    if (_listener) {
        _listener->onMouseReleased(button);
    }
}

void MouseInputComponent::update(float delta) {
}

void MouseInputComponent::render(const std::unique_ptr<Window> &window, const Viewport* viewport) {
}
