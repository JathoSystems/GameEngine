#ifndef GAMEENGINE_RECTANGLERENDERER_H
#define GAMEENGINE_RECTANGLERENDERER_H

#include "Component.h"
#include <SDL3/SDL.h>
#include <memory>

class RectangleRenderer : public Component {
private:
    SDL_Color _color;
    bool _filled;

public:
    RectangleRenderer(SDL_Color color = {0, 255, 0, 128}, bool filled = false);

    RectangleRenderer(int r, int g, int b, int a = 255, bool filled = false);

    void update(float delta) override;

    void render(const std::unique_ptr<Window> &window) override;

    void setColor(SDL_Color color);

    void setColor(int r, int g, int b, int a = 255);

    void setFilled(bool filled);
};

#endif //GAMEENGINE_RECTANGLERENDERER_H
