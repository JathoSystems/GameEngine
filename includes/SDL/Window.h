//
// Created by kikker234 on 08-11-2025.
//

#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <SDL_render.h>
#include <SDL_video.h>
#include <string>

class Window {
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;

public:
    ~Window();
    void openWindow(int width, int height, std::string name);

    SDL_Window * getWindow();
    SDL_Renderer* getRenderer();
};

#endif //GAMEENGINE_WINDOW_H