#ifndef GAMEENGINE_INPUTSYSTEM_H
#define GAMEENGINE_INPUTSYSTEM_H

#include "Engine/ISystem.h"
#include "IKeyListener.h"
#include "IMouseListener.h"
#include <SDL3/SDL.h>
#include <vector>
#include <memory>

#include "GameObjects/Transform/Position.h"

class KeyInputComponent;
class MouseInputComponent;

class InputSystem : public ISystem {
private:
    std::vector<KeyInputComponent *> _keyComponents;
    std::vector<MouseInputComponent *> _mouseComponents;

    Key sdlKeyToKey(SDL_Keycode sdlKey);

    MouseButton sdlButtonToButton(uint8_t sdlButton);

public:
    InputSystem() = default;

    void registerKeyComponent(KeyInputComponent *component);

    void registerMouseComponent(MouseInputComponent *component);

    void unregisterKeyComponent(KeyInputComponent *component);

    void unregisterMouseComponent(MouseInputComponent *component);

    void processInput();

    void update(float deltaTime) override;

    std::unique_ptr<Position> getMousePosition() const;
};

#endif
