#ifndef GAMEENGINE_KEYINPUTCOMPONENT_H
#define GAMEENGINE_KEYINPUTCOMPONENT_H

#include "GameObjects/Component/Component.h"
#include "../../Input/IKeyListener.h"
#include <memory>

class KeyInputComponent : public Component {
private:
    GameObject *_gameObject;
    IKeyListener *_listener;

public:
    explicit KeyInputComponent(GameObject *gameObject);
    ~KeyInputComponent();

    void setListener(IKeyListener *listener);

    void handleKeyPress(Key key);

    void handleKeyRelease(Key key);

    void update(float delta) override;

    void render(const std::unique_ptr<Window> &window) override;
};

#endif
