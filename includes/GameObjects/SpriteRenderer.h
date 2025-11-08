//
// Created by kikker234 on 08-11-2025.
//

#ifndef GAMEENGINE_SPRITERENDERER_H
#define GAMEENGINE_SPRITERENDERER_H

#include <string>
#include <SDL2/SDL.h>
#include <memory>

#include "Component.h"
#include "SDL/Window.h"

class SpriteRenderer : public Component {
private:
    std::string _spritePath;
    SDL_Texture* _texture = nullptr;
    SDL_Rect _rect{};

public:
    SpriteRenderer(std::string path);
    ~SpriteRenderer();

    void loadTexture(const std::unique_ptr<Window>& window);
    void update() override;
    void render(const std::unique_ptr<Window> &window) override;
};

#endif //GAMEENGINE_SPRITERENDERER_H
