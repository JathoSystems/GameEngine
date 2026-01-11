#ifndef GAMEENGINE_BUTTON_H
#define GAMEENGINE_BUTTON_H
#include <functional>
#include <memory>

#include "Text.h"

class Button : public Component {
private:
    int _paddingX = 20;
    int _paddingY = 10;
    bool _wasPressed = false;
    std::string _text;
    std::unique_ptr<Font> _font;
    std::unique_ptr<Color> _backgroundColor;
    std::unique_ptr<Color> _textColor;
    std::function<void()> _onClickCallback;

public:
    Button(std::string buttonText, std::unique_ptr<Color> backgroundColor);

    void setBackgroundColor(std::unique_ptr<Color> color);

    void setTextColor(std::unique_ptr<Color> color);

    void setFont(std::string path, std::string name);

    void setPadding(int paddingX, int paddingY);

    void update(float deltaTime) override;

    void render(const std::unique_ptr<Window> &window) override;

    void setOnClick(std::function<void()> callback);

    void onClick();
};

#endif //GAMEENGINE_BUTTON_H
