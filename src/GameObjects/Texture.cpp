//
// Created by jusra on 10-11-2025.
//
#include "GameObjects/Texture.h"

#include <iostream>

#include "SDL3_image/SDL_image.h"

void Texture::load(Window *window) {
    SDL_Renderer *renderer = window->getRenderer();
    if (!renderer)
        return;


    SDL_Surface *surface = IMG_Load(_path.c_str());
    if (!surface)
        return;


    _texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!_texture) {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_DestroySurface(surface);

    float width, height;
    if (!SDL_GetTextureSize(_texture, &width, &height)) {
        return;
    }

    _rectangle.w = width;
    _rectangle.h = height;
}

Texture::Texture(std::string path) : _path(path), _texture(nullptr) {
}

Texture::~Texture() {
    if (_texture) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

void Texture::render(Window *window) {
    SDL_Renderer *renderer = window->getRenderer();
    if (!renderer) {
        return;
    }

    if (!_texture)
        load(window);

    SDL_RenderTexture(renderer, _texture, nullptr, &_rectangle);
}

void Texture::transform(Transform *transform) {
    if (!transform)
        return;

    _rectangle.x = transform->getPosition()->getX();
    _rectangle.y = transform->getPosition()->getY();
    _rectangle.w = transform->getSize()->getWidth();
    _rectangle.h = transform->getSize()->getHeight();
}

SDL_Texture *Texture::getTexture(Window *window) {
    if (_texture == nullptr)
        load(window);

    return _texture;
}

int Texture::getWidth() {
    return _texture ? _rectangle.w : 0;
}

int Texture::getHeight() {
    return _texture ? _rectangle.h : 0;
}
