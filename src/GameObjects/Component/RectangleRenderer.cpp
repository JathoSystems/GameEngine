#include "GameObjects/Component/RectangleRenderer.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"
#include "SDL/Window.h"

RectangleRenderer::RectangleRenderer(SDL_Color color, bool filled) 
    : _color(color), _filled(filled) {
}

RectangleRenderer::RectangleRenderer(int r, int g, int b, int a, bool filled) 
    : _filled(filled) {
    _color = {static_cast<Uint8>(r), static_cast<Uint8>(g), 
              static_cast<Uint8>(b), static_cast<Uint8>(a)};
}

void RectangleRenderer::update(float delta) {
    // No update logic needed for simple rectangle rendering
}

void RectangleRenderer::render(const std::unique_ptr<Window> &window) {
    if (!_parent) return;

    Transform* transform = _parent->getTransform();
    if (!transform) return;

    Position* pos = transform->getPosition();
    Size* size = transform->getSize();

    if (!pos || !size) return;

    SDL_Renderer* renderer = window->getRenderer();
    if (!renderer) return;

    // Create rectangle from transform
    SDL_FRect rect;
    rect.x = static_cast<float>(pos->getX());
    rect.y = static_cast<float>(pos->getY());
    rect.w = static_cast<float>(size->getWidth());
    rect.h = static_cast<float>(size->getHeight());

    // Set render color
    SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);

    // Draw filled or outline
    if (_filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderRect(renderer, &rect);
    }
}

void RectangleRenderer::setColor(SDL_Color color) {
    _color = color;
}

void RectangleRenderer::setColor(int r, int g, int b, int a) {
    _color = {static_cast<Uint8>(r), static_cast<Uint8>(g), 
              static_cast<Uint8>(b), static_cast<Uint8>(a)};
}

void RectangleRenderer::setFilled(bool filled) {
    _filled = filled;
}