//
// Created by jusra on 11-11-2025.
//

#ifndef GAMEENGINE_COLOR_H
#define GAMEENGINE_COLOR_H
#include <string>

#include "SDL3/SDL_pixels.h"

class Color {
private:
    int _r = 255;
    int _g = 0;
    int _b = 0;

public:
    Color(int r, int g, int b);
    Color(const std::string& hex);

    std::string toHex();
    void setFromHex(const std::string& hex);

    SDL_Color toSdlColor();
};

#endif //GAMEENGINE_COLOR_H