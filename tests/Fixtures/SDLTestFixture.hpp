//
// Created by jusra on 27-11-2025.
//
#ifndef GAMEENGINE_SDLTESTFIXTURE_HPP
#define GAMEENGINE_SDLTESTFIXTURE_HPP
#include <SDL3/SDL_init.h>

struct SDLTestFixture {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDLTestFixture() {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        window = SDL_CreateWindow("Test", 100, 100, SDL_WINDOW_HIDDEN);
        renderer = SDL_CreateRenderer(window, "Testing");
    }

    ~SDLTestFixture() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
};
#endif //GAMEENGINE_SDLTESTFIXTURE_HPP