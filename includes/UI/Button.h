//
// Created by jusra on 11-11-2025.
//
#ifndef GAMEENGINE_BUTTON_H
#define GAMEENGINE_BUTTON_H
#include <memory>

#include "Text.h"

class Button : public Component {
private:
    std::unique_ptr<Text> _text;
    std::unique_ptr<Color> _backgroundColor;

public:
    Button(std::unique_ptr<Text> text, std::unique_ptr<Color> backgroundColor);
    void update(float deltaTime) override;
    void render(const std::unique_ptr<Window> &window) override;
};

#endif //GAMEENGINE_BUTTON_H