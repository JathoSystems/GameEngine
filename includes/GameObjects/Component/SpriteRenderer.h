//
// Created by kikker234 on 08-11-2025.
//

#ifndef GAMEENGINE_SPRITERENDERER_H
#define GAMEENGINE_SPRITERENDERER_H

#include <string>
#include <SDL3/SDL.h>
#include <memory>

#include "Component.h"
#include "Scenes/Camera/Viewport.h"
#include "../Texture.h"
#include "SDL/Window.h"

class SpriteRenderer : public Component {
private:
    std::unique_ptr<Texture> _texture;

public:
    SpriteRenderer(std::string path);

    void loadTexture(const std::unique_ptr<Window>& window);
    void update(float delta) override;
    void render(const std::unique_ptr<Window> &window, const Viewport* viewport) override;
};

#endif //GAMEENGINE_SPRITERENDERER_H
