#include <iostream>
#include "GameObjects/GameObject.h"
#include "GameObjects/Behaviour.h"
#include "Scenes/Camera/FixedCamera.h"

class FixedCamera;

class SpriteMovement : public Behaviour {
private:
    int _direction = 1;

    void onUpdate() override {
        Size* size = _parent->getTransform()->getSize();
        Position* pos = _parent->getTransform()->getPosition();
        pos->setX(pos->getX() + _direction);

        if ((pos->getX() + size->getWidth()) >= 1000 || pos->getX() <= 0)
            _direction = -_direction;
    };
};

class SpriteRotator : public Behaviour {
private:
    int _rot = 0;

    void onUpdate() override {
        Rotation* rotation = _parent->getTransform()->getRotation();
        _rot++;
        if (_rot >= 360) _rot = 0;
        rotation->setRotation(_rot);
    };
};

int main() {
    std::cout << "hello Game Engine" << std::endl;

    return 0;
}

