//
// Created by kikker234 on 08-11-2025.
//

#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <SDL3/SDL.h>
#include <string>
#include "Scenes/Camera/Viewport.h"

class Window {
private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    const Viewport* _activeViewport = nullptr;

public:
    ~Window();
    void openWindow(int width, int height, std::string name, int flags = 0);

    SDL_Window * getWindow();
    SDL_Renderer* getRenderer();

    void setActiveViewport(const Viewport* viewport);
    const Viewport* getActiveViewport() const;
};

#endif //GAMEENGINE_WINDOW_H