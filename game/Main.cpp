#include <memory>

#include "../includes/Engine/GameEngine.h"
#include "../includes/GameObjects/GameObject.h"
#include "../includes/Scenes/Scene.h"
#include "GameObjects/Behaviour.h"
#include "GameObjects/SpriteRenderer.h"

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
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 1000, 500);

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();

    std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/Watergirl.png");
    object->addComponent(std::move(sprite));
    object->addComponent(std::make_unique<SpriteRotator>());
    scene->addObject(std::move(object));

    std::unique_ptr<GameObject> object2 = std::make_unique<GameObject>();
    std::unique_ptr<SpriteRenderer> watergirl = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    object2->addComponent(std::make_unique<SpriteMovement>());
    object2->addComponent(std::move(watergirl));
    scene->addObject(std::move(object2));

    engine->addScene(std::move(scene));

    engine->start();
}