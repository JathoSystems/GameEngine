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
    int _a = 255;

public:
    Color(int r, int g, int b);
    Color(const std::string& hex);

    std::string toHex();
    void setFromHex(const std::string& hex);

    SDL_Color toSdlColor();

    int getR() const;
    void setR(int r);
    int getG() const;
    void setG(int g);
    int getB() const;
    void setB(int b);
    int getA() const;
    void setA(int a);
};

#endif //GAMEENGINE_COLOR_H