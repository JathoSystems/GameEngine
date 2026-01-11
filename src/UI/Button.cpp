//
// Created by jusra on 11-11-2025.
//
#include "UI/Button.h"
#include <iostream>

Button::Button(std::string buttonText, std::unique_ptr<Color> backgroundColor) {
    _text = buttonText;
    _font = Font::getDefaultFont();
    _backgroundColor = std::move(backgroundColor);
    _textColor = std::make_unique<Color>(255, 255, 255);
}

void Button::setBackgroundColor(std::unique_ptr<Color> color) {
    _backgroundColor = std::move(color);
}

void Button::setTextColor(std::unique_ptr<Color> color) {
    _textColor = std::move(color);
}

void Button::setFont(std::string path, std::string name) {
    _font = std::make_unique<Font>(path, name);
}

void Button::setPadding(int paddingX, int paddingY) {
    _paddingX = paddingX;
    _paddingY = paddingY;
}

void Button::update(float deltaTime) {
    if (!_parent) return;

    Transform* transform = _parent->getTransform();
    if (!transform) return;

    SDL_FRect rect = transform->toFRect();

    if (rect.w == 0 || rect.h == 0) {
        if (_font && !_text.empty()) {
            SDL_Surface* surface = TTF_RenderText_Solid(_font->getSdlFont(), _text.c_str(), 0, _textColor->toSdlColor());
            if (surface) {
                if (rect.w == 0) rect.w = surface->w + (_paddingX * 2);
                if (rect.h == 0) rect.h = surface->h + (_paddingY * 2);
                SDL_DestroySurface(surface);
            }
        }

        if (rect.w == 0) rect.w = 100.0f;
        if (rect.h == 0) rect.h = 40.0f;
    }

    float mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    bool isMouseOver = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                        mouseY >= rect.y && mouseY <= rect.y + rect.h);

    bool isPressed = (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK) != 0;

    if (isMouseOver && isPressed && !_wasPressed) {
        onClick();
    }

    _wasPressed = isPressed;
}

void Button::render(const std::unique_ptr<Window> &window) {
    if (!window || !_backgroundColor) return;

    SDL_Renderer *renderer = window->getRenderer();
    if (!renderer) return;

    Transform *transform = _parent->getTransform();
    if (!transform) return;

    SDL_FRect rect = transform->toFRect();

    // Check font and text BEFORE creating surface
    if (!_font || !_font->getSdlFont() || _text.empty()) {
        // Still render the background even without text
        SDL_SetRenderDrawColor(renderer,
                               _backgroundColor->getR(),
                               _backgroundColor->getG(),
                               _backgroundColor->getB(),
                               _backgroundColor->getA());
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(_font->getSdlFont(), _text.c_str(), 0, _textColor->toSdlColor());
    if (!surface) {
        // Render background even if text rendering failed
        SDL_SetRenderDrawColor(renderer,
                               _backgroundColor->getR(),
                               _backgroundColor->getG(),
                               _backgroundColor->getB(),
                               _backgroundColor->getA());
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    if (rect.w == 0) rect.w = surface->w + (_paddingX * 2);
    if (rect.h == 0) rect.h = surface->h + (_paddingY * 2);

    SDL_SetRenderDrawColor(renderer,
                           _backgroundColor->getR(),
                           _backgroundColor->getG(),
                           _backgroundColor->getB(),
                           _backgroundColor->getA());

    SDL_RenderFillRect(renderer, &rect);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return;
    }

    float textX = rect.x + (rect.w - surface->w) / 2.0f;
    float textY = rect.y + (rect.h - surface->h) / 2.0f;

    SDL_FRect textRect = {
        textX,
        textY,
        static_cast<float>(surface->w),
        static_cast<float>(surface->h)
    };

    SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void Button::setOnClick(std::function<void()> callback) {
    _onClickCallback = callback;
}

void Button::onClick() {
    if (_onClickCallback) {
        _onClickCallback();
    }
}
