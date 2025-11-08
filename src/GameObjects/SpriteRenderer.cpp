//
// Created by kikker234 on 08-11-2025.
//
#include "GameObjects/SpriteRenderer.h"
#include "SDL/Window.h"

#include <SDL2/SDL_image.h>
#include <iostream>

#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"

SpriteRenderer::SpriteRenderer(std::string path)
    : _spritePath(std::move(path)) {}

SpriteRenderer::~SpriteRenderer() {
    if (_texture) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

void SpriteRenderer::loadTexture(const std::unique_ptr<Window>& window) {
    SDL_Renderer* renderer = window->getRenderer();
    SDL_Surface* surface = IMG_Load(_spritePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << _spritePath
                  << " | SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    _texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!_texture) {
        std::cerr << "Failed to create texture from " << _spritePath
                  << " | SDL Error: " << SDL_GetError() << std::endl;
    }

    // Standaard rect — hele texture
    int width;
    int height;
    SDL_QueryTexture(_texture, nullptr, nullptr, &width, &height);

    if (_parent->getTransform()) {
        _parent->getTransform()->getSize()->setWidth(width);
        _parent->getTransform()->getSize()->setHeight(height);
    }
}

void SpriteRenderer::update() {
    Transform* transform = _parent->getTransform().get();
    if (!transform || !_texture) return;

    _rect.x = transform->getPosition()->getX();
    _rect.y = transform->getPosition()->getY();

    _rect.w = transform->getSize()->getWidth();
    _rect.h = transform->getSize()->getHeight();
}

void SpriteRenderer::render(const std::unique_ptr<Window>& window) {
    if (!_texture) {
        loadTexture(window);
    }

    SDL_Renderer* renderer = window->getRenderer();
    if (renderer && _texture) {
        SDL_Point center = { _rect.w / 2, _rect.h / 2 }; // roteren om het midden
        SDL_RenderCopyEx(renderer, _texture, nullptr, &_rect, _parent->getTransform()->getRotation()->getRotation(), &center, SDL_FLIP_NONE);
    }
}
