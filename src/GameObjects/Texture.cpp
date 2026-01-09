#include "GameObjects/Texture.h"
#include "GameObjects/GameObject.h"
#include "Scenes/Camera/Viewport.h"
#include "SDL3_image/SDL_image.h"
#include <iostream>

void Texture::load(Window *window) {
    SDL_Renderer *renderer = window->getRenderer();
    if (!renderer)
        return;

    SDL_Surface *surface = IMG_Load(_path.c_str());
    if (!surface) {
        // std::cerr << "Failed to load image!" << std::endl;
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

    const Viewport* viewport = window->getActiveViewport();
    if (!viewport) {
        std::cout << "ViewPort null" << std::endl;
        return;
    }

    if (!_texture)
        load(window);

    SDL_FRect screenRect = _rectangle;
    if (viewport) {
        Position viewportPos = viewport->getPosition();
        screenRect.x = _rectangle.x - viewportPos.getX();
        screenRect.y = _rectangle.y - viewportPos.getY();
    }

    SDL_RenderTexture(renderer, _texture, nullptr, &screenRect);
}


void Texture::render(Window* window, Frame* frame, GameObject* parent) {
    if (!_texture || !frame) {
        load(window);
    }
    if (!frame || !parent) return;

    Transform* transform = parent->getTransform();
    if (!transform) return;

    const Viewport* viewport = window->getActiveViewport();

    Size* size = transform->getSize();
    Position* pos = transform->getPosition();
    if (!size || !pos) return;

    float width = size->getWidth() == 0 ? frame->getWidth() : size->getWidth();
    float height = size->getHeight() == 0 ? frame->getHeight() : size->getHeight();

    SDL_FRect dstRect;
    // Transform position is CENTER point (same as Box2D physics)
    // Calculate top-left corner for rendering
    dstRect.x = pos->getX() - width / 2.0f;
    dstRect.y = pos->getY() - height / 2.0f;

    // Apply viewport offset
    if (viewport) {
        Position viewportPos = viewport->getPosition();
        dstRect.x -= viewportPos.getX();
        dstRect.y -= viewportPos.getY();
    }

    dstRect.w = width;
    dstRect.h = height;

    SDL_FRect srcRect;
    srcRect.x = frame->getX();
    srcRect.y = frame->getY();
    srcRect.w = frame->getWidth();
    srcRect.h = frame->getHeight();

    SDL_RenderTexture(window->getRenderer(), _texture, &srcRect, &dstRect);
}

void Texture::transform(Transform *transform) {
    if (!transform) return;

    Size* size = transform->getSize();
    Position* pos = transform->getPosition();
    if (!size || !pos) return;

    float width = size->getWidth();
    float height = size->getHeight();

    _rectangle.x = pos->getX() - width / 2.0f;
    _rectangle.y = pos->getY() - height / 2.0f;
    _rectangle.w = width;
    _rectangle.h = height;
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