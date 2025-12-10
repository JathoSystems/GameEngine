#ifndef GAMEENGINE_FPSCOUNTER_H
#define GAMEENGINE_FPSCOUNTER_H

#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include "Color.h"
#include "Font.h"
#include "SDL/Window.h"

class FPSCounter {
private:
    Uint64 _lastTime;
    int _frameCount;
    int _currentFPS;
    bool _isVisible;
    std::unique_ptr<Font> _font;
    std::unique_ptr<Color> _color;
    float _x, _y, _width, _height;

public:
    FPSCounter();
    ~FPSCounter() = default;

    void update(float deltaTime);
    void render(const std::unique_ptr<Window>& window);

    void setPosition(float x, float y);
    void setSize(float width, float height);

    void toggleVisibility();
    void setVisible(bool visible);
    bool isVisible() const { return _isVisible; }
    int getFPS() const { return _currentFPS; }

    void setFont(std::string path, std::string fontName);
    void setColor(std::unique_ptr<Color> color);
    void setFontSize(int size);
};

#endif //GAMEENGINE_FPSCOUNTER_H
