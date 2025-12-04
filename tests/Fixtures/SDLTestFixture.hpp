#ifndef GAMEENGINE_SDLTESTFIXTURE_HPP
#define GAMEENGINE_SDLTESTFIXTURE_HPP
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "SDL/Window.h"
#include <memory>

struct SDLTestFixture {
    std::unique_ptr<Window> testWindow;

    SDLTestFixture() {
        TTF_Init();
        testWindow = std::make_unique<Window>();
        testWindow->openWindow(100, 100, "Test", SDL_WINDOW_HIDDEN);
    }

    ~SDLTestFixture() {
        testWindow.reset();
        TTF_Quit();
        SDL_Quit();
    }
};
#endif