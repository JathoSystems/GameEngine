#pragma once
#include <memory>

#include "Viewport.h"

class Camera {
private:
    std::unique_ptr<Viewport> _viewport;

public:
    Camera(std::unique_ptr<Viewport> port);
    const Viewport* getViewPort() const;
    virtual Position getPosition() const = 0;
};

