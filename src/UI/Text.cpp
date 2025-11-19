//
// Created by jusra on 11-11-2025.
//
#include "UI/Text.h"
#include <iostream>
#include <cstring>


Text::Text(const std::string &text) {
    _text = text;
    _color = std::make_unique<Color>(255, 255, 255);
    _font = Font::getDefaultFont();
}

Text::~Text() {
}

void Text::setFont(std::string path, std::string fontName) {
    _font = std::make_unique<Font>(path, fontName);
}

void Text::setColor(std::unique_ptr<Color> color) {
    _color = std::move(color);
}

void Text::setFontSize(int size) {
    _font->setSize(size);
}

void Text::render(const std::unique_ptr<Window>& window) {
    if (!window || !window->getRenderer() || !_font || !_color || !_parent) return;

    TTF_Font* sdlFont = _font->getSdlFont();
    if (!sdlFont) return;

    SDL_Color color = _color->toSdlColor();

    SDL_Surface* surface = TTF_RenderText_Blended(sdlFont, _text.c_str(), 0, color);
    if (!surface) {
        std::cerr << "TTF_RenderUTF8_Blended failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(window->getRenderer(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << "\n";
        SDL_DestroySurface(surface);
        return;
    }

    Size* size = _parent->getTransform()->getSize();
    if (size->getWidth() == 0) size->setWidth(surface->w);
    if (size->getHeight() == 0) size->setWidth(surface->h);
    SDL_FRect dest = _parent->getTransform()->toFRect();

    SDL_RenderTexture(window->getRenderer(), texture, nullptr, &dest);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}


void Text::update(float deltaTime) {
    (void)deltaTime;
}