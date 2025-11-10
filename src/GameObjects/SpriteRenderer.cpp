//
// Created by kikker234 on 08-11-2025.
//
#include "GameObjects/SpriteRenderer.h"
#include "SDL/Window.h"

#include <iostream>

#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"
#include "SDL3_image/SDL_image.h"

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

    // SDL3: IMG_Load returns SDL_Surface* (same as SDL2)
    SDL_Surface* surface = IMG_Load(_spritePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << _spritePath << std::endl;
        return;
    }

    // SDL3: SDL_CreateTextureFromSurface works the same
    _texture = SDL_CreateTextureFromSurface(renderer, surface);

    // SDL3: SDL_DestroySurface instead of SDL_FreeSurface
    SDL_DestroySurface(surface);

    if (!_texture) {
        std::cerr << "Failed to create texture from " << _spritePath
                  << " | SDL Error: " << SDL_GetError() << std::endl;
        return;  // Added return to prevent using null texture
    }

    // SDL3: SDL_GetTextureSize instead of SDL_QueryTexture for size
    float width, height;  // Note: returns float in SDL3
    if (!SDL_GetTextureSize(_texture, &width, &height)) {
        std::cerr << "Failed to query texture size | SDL Error: "
                  << SDL_GetError() << std::endl;
        return;
    }

    if (_parent->getTransform()) {
        _parent->getTransform()->getSize()->setWidth(static_cast<int>(width));
        _parent->getTransform()->getSize()->setHeight(static_cast<int>(height));
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
        // SDL3: SDL_FPoint instead of SDL_Point (uses float)
        SDL_FPoint center = { _rect.w / 2.0f, _rect.h / 2.0f };

        // SDL3: SDL_RenderTextureRotated instead of SDL_RenderCopyEx
        // Note: SDL_FLIP_NONE is now SDL_FLIP_NONE (same name, but check enum)
        SDL_RenderTextureRotated(
            renderer,
            _texture,
            nullptr,           // source rect
            &_rect,            // destination rect
            _parent->getTransform()->getRotation()->getRotation(),
            &center,
            SDL_FLIP_NONE
        );
    }
}