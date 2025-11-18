#pragma once
#include <memory>

#include "Viewport.h"

class Camera {
private:
    std::unique_ptr<Viewport> _viewport;

public:
    Camera(std::unique_ptr<Viewport> port);
    Viewport* getViewPort();
    virtual Position getPosition() const = 0;
};

