//
// Created by jusra on 11-11-2025.
//

#ifndef GAMEENGINE_TEXT_H
#define GAMEENGINE_TEXT_H
#include "Color.h"
#include "Font.h"
#include "UiElement.h"

class Text : public UiElement {
private:
    std::unique_ptr<Font> _font;
    std::string _text;
    std::unique_ptr<Color> _color;

public:
    Text(const std::string& text);

    ~Text();

    void setFont(std::string path, std::string fontName);
    void setColor(std::unique_ptr<Color> color);
    void setFontSize(int size);
    void render(const std::unique_ptr<Window> &window) override;
    void update(float deltaTime) override;
};

#endif //GAMEENGINE_TEXT_H
