#include "GameObjects/Component/KeyInputComponent.h"

KeyInputComponent::KeyInputComponent(GameObject *gameObject)
    : _gameObject(gameObject), _listener(nullptr) {
}

void KeyInputComponent::setListener(IKeyListener *listener) {
    _listener = listener;
}

void KeyInputComponent::handleKeyPress(Key key) {
    if (_listener) {
        _listener->onKeyPress(key);
    }
}

void KeyInputComponent::handleKeyRelease(Key key) {
    if (_listener) {
        _listener->onKeyRelease(key);
    }
}

void KeyInputComponent::update(float delta) {
}

void KeyInputComponent::render(const std::unique_ptr<Window> &window, const Viewport* viewport) {
}
