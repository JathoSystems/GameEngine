#include "Input/InputSystem.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "GameObjects/Component/MouseInputComponent.h"
#include <algorithm>
#include <unordered_map>

// Ugly maar nodig
static const std::unordered_map<SDL_Keycode, Key> SDL_TO_KEY_MAP = {
    {SDLK_A, Key::A}, {SDLK_B, Key::B}, {SDLK_C, Key::C}, {SDLK_D, Key::D},
    {SDLK_E, Key::E}, {SDLK_F, Key::F}, {SDLK_G, Key::G}, {SDLK_H, Key::H},
    {SDLK_I, Key::I}, {SDLK_J, Key::J}, {SDLK_K, Key::K}, {SDLK_L, Key::L},
    {SDLK_M, Key::M}, {SDLK_N, Key::N}, {SDLK_O, Key::O}, {SDLK_P, Key::P},
    {SDLK_Q, Key::Q}, {SDLK_R, Key::R}, {SDLK_S, Key::S}, {SDLK_T, Key::T},
    {SDLK_U, Key::U}, {SDLK_V, Key::V}, {SDLK_W, Key::W}, {SDLK_X, Key::X},
    {SDLK_Y, Key::Y}, {SDLK_Z, Key::Z},
    {SDLK_0, Key::NUM_0}, {SDLK_1, Key::NUM_1}, {SDLK_2, Key::NUM_2},
    {SDLK_3, Key::NUM_3}, {SDLK_4, Key::NUM_4}, {SDLK_5, Key::NUM_5},
    {SDLK_6, Key::NUM_6}, {SDLK_7, Key::NUM_7}, {SDLK_8, Key::NUM_8},
    {SDLK_9, Key::NUM_9},
    {SDLK_SPACE, Key::SPACE}, {SDLK_RETURN, Key::ENTER}, {SDLK_ESCAPE, Key::ESCAPE},
    {SDLK_TAB, Key::TAB}, {SDLK_BACKSPACE, Key::BACKSPACE}, {SDLK_DELETE, Key::DELETE},
    {SDLK_LEFT, Key::LEFT}, {SDLK_RIGHT, Key::RIGHT}, {SDLK_UP, Key::UP}, {SDLK_DOWN, Key::DOWN},
    {SDLK_LSHIFT, Key::SHIFT_LEFT}, {SDLK_RSHIFT, Key::SHIFT_RIGHT},
    {SDLK_LCTRL, Key::CTRL_LEFT}, {SDLK_RCTRL, Key::CTRL_RIGHT},
    {SDLK_LALT, Key::ALT_LEFT}, {SDLK_RALT, Key::ALT_RIGHT},
    {SDLK_F1, Key::F1}, {SDLK_F2, Key::F2}, {SDLK_F3, Key::F3}, {SDLK_F4, Key::F4},
    {SDLK_F5, Key::F5}, {SDLK_F6, Key::F6}, {SDLK_F7, Key::F7}, {SDLK_F8, Key::F8},
    {SDLK_F9, Key::F9}, {SDLK_F10, Key::F10}, {SDLK_F11, Key::F11}, {SDLK_F12, Key::F12}
};

void InputSystem::registerKeyComponent(KeyInputComponent *component) {
    _keyComponents.push_back(component);
}

void InputSystem::registerMouseComponent(MouseInputComponent *component) {
    _mouseComponents.push_back(component);
}

void InputSystem::unregisterKeyComponent(KeyInputComponent *component) {
    auto it = std::find(_keyComponents.begin(), _keyComponents.end(), component);
    if (it != _keyComponents.end()) {
        _keyComponents.erase(it);
    }
}

void InputSystem::unregisterMouseComponent(MouseInputComponent *component) {
    auto it = std::find(_mouseComponents.begin(), _mouseComponents.end(), component);
    if (it != _mouseComponents.end()) {
        _mouseComponents.erase(it);
    }
}

Key InputSystem::sdlKeyToKey(SDL_Keycode sdlKey) {
    auto it = SDL_TO_KEY_MAP.find(sdlKey);
    return (it != SDL_TO_KEY_MAP.end()) ? it->second : Key::SPACE;
}



MouseButton InputSystem::sdlButtonToButton(uint8_t sdlButton) {
    switch (sdlButton) {
        case SDL_BUTTON_LEFT: return MouseButton::LEFT;
        case SDL_BUTTON_MIDDLE: return MouseButton::MIDDLE;
        case SDL_BUTTON_RIGHT: return MouseButton::RIGHT;
        case SDL_BUTTON_X1: return MouseButton::X1;
        case SDL_BUTTON_X2: return MouseButton::X2;
        default: return MouseButton::LEFT;
    }
}

void InputSystem::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                SDL_Quit();
                exit(0);
                break;
            case SDL_EVENT_KEY_DOWN: {
                Key key = sdlKeyToKey(event.key.key);
                for (auto *component: _keyComponents) {
                    component->handleKeyPress(key);
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                Key key = sdlKeyToKey(event.key.key);
                for (auto *component: _keyComponents) {
                    component->handleKeyRelease(key);
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                MouseButton button = sdlButtonToButton(event.button.button);
                for (auto *component: _mouseComponents) {
                    component->handleMousePressed(button);
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                MouseButton button = sdlButtonToButton(event.button.button);
                for (auto *component: _mouseComponents) {
                    component->handleMouseReleased(button);
                }
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                for (auto *component: _mouseComponents) {
                    int x = event.motion.x;
                    int y = event.motion.y;
                    component->handleMouseMoved(std::make_unique<Position>(x, y));
                }
                break;
            }
        }
    }
}

void InputSystem::update(float deltaTime) {
    processInput();
}