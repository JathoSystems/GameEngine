#include <memory>

#include "../includes/Engine/GameEngine.h"
#include "../includes/GameObjects/GameObject.h"
#include "../includes/Scenes/Scene.h"
#include "GameObjects/Behaviour.h"
#include "GameObjects/SpriteRenderer.h"

class SpriteMovement : public Behaviour {
private:
    int _direction = 1;
    int _rot = 0;

    void onUpdate() override {
        Size* size = _parent->getTransform()->getSize();
        Position* pos = _parent->getTransform()->getPosition();
        Rotation* rotation = _parent->getTransform()->getRotation();
        pos->setX(pos->getX() + _direction);

        if ((pos->getX() + size->getWidth()) >= 500 || pos->getX() <= 0)
            _direction = -_direction;

        _rot++;
        if (_rot >= 360) _rot = 0;

        rotation->setRotation(_rot);
    };
};

int main() {
    std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
    engine->init("Vuurjongen en watermeisje", 500, 500);

    std::unique_ptr<Scene> scene = std::make_unique<Scene>("main");
    std::unique_ptr<GameObject> object = std::make_unique<GameObject>();
    object->getTransform()->getSize()->setWidth(172);
    object->getTransform()->getSize()->setHeight(293);

    std::unique_ptr<SpriteRenderer> sprite = std::make_unique<SpriteRenderer>("../resources/sprite.jpeg");
    sprite->loadTexture(engine->getWindow());
    object->addComponent(std::move(sprite));
    object->addComponent(std::make_unique<SpriteMovement>());
    scene->addObject(std::move(object));
    engine->addScene(std::move(scene));

    engine->start();
}