#include "UI/FPSCounter.h"
#include <sstream>

FPSCounter::FPSCounter()
    : _lastTime(SDL_GetTicks()), _frameCount(0),
      _currentFPS(0), _isVisible(true),
      _x(0), _y(0), _width(100), _height(30) {
    _font = Font::getDefaultFont();
    _color = std::make_unique<Color>(255, 255, 255);
}

void FPSCounter::update(float deltaTime) {
    _frameCount++;
    Uint64 currentTime = SDL_GetTicks();

    if (currentTime - _lastTime >= 1000) {
        _currentFPS = _frameCount;
        _frameCount = 0;
        _lastTime = currentTime;
    }
}

void FPSCounter::render(const std::unique_ptr<Window> &window) {
    if (!_isVisible) return;

    std::stringstream ss;
    ss << "FPS: " << _currentFPS;
    std::string fpsText = ss.str();

    SDL_Surface *surface = TTF_RenderText_Blended(
        _font->getSdlFont(),
        fpsText.c_str(),
        fpsText.length(),
        _color->toSdlColor()
    );

    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
    SDL_DestroySurface(surface);

    if (!texture) return;

    SDL_FRect rect = {_x, _y, _width, _height};
    SDL_RenderTexture(window->getRenderer(), texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}


void FPSCounter::setPosition(float x, float y) {
    _x = x;
    _y = y;
}

void FPSCounter::setSize(float width, float height) {
    _width = width;
    _height = height;
}

void FPSCounter::toggleVisibility() {
    _isVisible = !_isVisible;
}

void FPSCounter::setVisible(bool visible) {
    _isVisible = visible;
}

void FPSCounter::setFont(std::string path, std::string fontName) {
    _font = std::make_unique<Font>(path, fontName);
}

void FPSCounter::setColor(std::unique_ptr<Color> color) {
    _color = std::move(color);
}

void FPSCounter::setFontSize(int size) {
    _font->setSize(size);
}
