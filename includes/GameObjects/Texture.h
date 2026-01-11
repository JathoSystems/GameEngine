#ifndef GAMEENGINE_TEXTURE_H
#define GAMEENGINE_TEXTURE_H
#include <string>
#include "GameObject.h"
#include "SDL/Window.h"
#include "SDL3/SDL_render.h"
#include "Spritesheet/Frame.h"
#include "Transform/Transform.h"

class Texture {
private:
    std::string _path;
    SDL_Texture *_texture;
    SDL_FRect _rectangle;

public:
    Texture(std::string path);

    ~Texture();

    void load(Window *window);

    void render(Window *window);

    void render(Window *window, Frame *frame, GameObject *parent);

    void transform(Transform *transform);

    SDL_Texture *getTexture(Window *window);

    int getWidth();

    int getHeight();
};

#endif //GAMEENGINE_TEXTURE_H
