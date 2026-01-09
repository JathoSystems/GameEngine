#include "GameObjects/Component/SpriteRenderer.h"
#include "SDL/Window.h"

#include <iostream>

#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"
#include "SDL3_image/SDL_image.h"

SpriteRenderer::SpriteRenderer(std::string path) {
    _texture = std::make_unique<Texture>(path);
}

void SpriteRenderer::update(float delta) {
    if (!_parent || !_texture) return;

    Transform *transform = _parent->getTransform();
    if (!transform) return;

    Size* size = transform->getSize();
    if (!size) return;

    if (size->getHeight() == 0)
        size->setHeight(_texture->getHeight());

    if (size->getWidth() == 0)
        size->setWidth(_texture->getWidth());

    _texture->transform(_parent->getTransform());
}

void SpriteRenderer::render(const std::unique_ptr<Window> &window) {
    _texture->render(window.get());
}