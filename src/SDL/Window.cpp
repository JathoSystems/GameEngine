#include "SDL/Window.h"
#include <iostream>
#include <SDL3/SDL_init.h>

void Window::openWindow(int width, int height, std::string name, int flags) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    _window = SDL_CreateWindow(
        name.c_str(),
        width,
        height,
        flags
    );

    if (!_window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return;
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return;
    }
}

SDL_Window *Window::getWindow() {
    return _window;
}

SDL_Renderer *Window::getRenderer() {
    return _renderer;
}

void Window::setActiveViewport(const Viewport *viewport) {
    _activeViewport = viewport;
}

const Viewport * Window::getActiveViewport() const {
    return _activeViewport;
}

Window::~Window() {
    if (_renderer) SDL_DestroyRenderer(_renderer);
    if (_window) SDL_DestroyWindow(_window);
    SDL_Quit();
}
