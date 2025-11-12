//
// Created by jusra on 10-11-2025.
//
#include "GameObjects/Texture.h"

#include <iostream>

#include "GameObjects/GameObject.h"
#include "SDL3_image/SDL_image.h"

void Texture::load(Window *window) {
    SDL_Renderer *renderer = window->getRenderer();
    if (!renderer)
        return;


    SDL_Surface *surface = IMG_Load(_path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image!" << std::endl;
        return;
    }

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

void Texture::render(Window* window, Frame* frame, GameObject* parent) {
    if (!_texture || !frame) {
        load(window);
    }
    if (!frame) return;


    Size* size = parent->getTransform()->getSize();
    SDL_FRect dstRect;
    dstRect.x = parent->getTransform()->getPosition()->getX();
    dstRect.y = parent->getTransform()->getPosition()->getY();
    dstRect.w = size->getWidth() == 0 ? frame->getWidth() : size->getWidth();
    dstRect.h = size->getHeight() == 0 ? frame->getHeight() : size->getHeight();

    SDL_FRect srcRect;
    srcRect.x = frame->getX();
    srcRect.y = frame->getY();
    srcRect.w = frame->getWidth();
    srcRect.h = frame->getHeight();

    SDL_RenderTexture(window->getRenderer(), _texture, &srcRect, &dstRect);
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
