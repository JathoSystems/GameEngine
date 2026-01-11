#include "Scenes/Camera/Camera.h"

Camera::Camera(std::unique_ptr<Viewport> port) {
    if (port != nullptr) {
        _viewport = std::move(port);
    }
}

Viewport *Camera::getViewPort() {
    return _viewport.get();
}
