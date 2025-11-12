//
// Created by jusra on 11-11-2025.
//
#include "UI/Text.h"
#include <iostream>
#include <cstring>

Text::Text(const std::string& text, std::unique_ptr<Font> font, std::unique_ptr<Color> color)
    : _text(text), _font(std::move(font)), _color(std::move(color)) {
}

Text::~Text() {
}

void Text::setFontSize(int size) {
    _font->setSize(size);
}

void Text::render(const std::unique_ptr<Window> &window) {
    if (!window || !window->getRenderer() || !_font) {
        return;
    }

    TTF_Font* sdlFont = _font->getSdlFont();
    if (!sdlFont)
        return;

    SDL_Surface* surface = TTF_RenderText_Blended(sdlFont, _text.c_str(), 0, _color->toSdlColor());

    if (!surface) {
        return;
    }
    const int surfaceWidth = surface->w;
    const int surfaceHeight = surface->h;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);

    SDL_DestroySurface(surface);

    if (!texture)
        return;

    Size* size = _parent->getTransform()->getSize();
    if (size->getWidth() == 0) size->setWidth(surfaceWidth);
    if (size->getHeight() == 0) size->setHeight(surfaceHeight);

    SDL_FRect dest = _parent->getTransform()->toFRect();
    SDL_RenderTexture(window->getRenderer(), texture, nullptr, &dest);
    SDL_DestroyTexture(texture);
}

void Text::update(float deltaTime) {
    (void)deltaTime;
}