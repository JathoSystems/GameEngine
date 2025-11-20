//
// Created by jusra on 11-11-2025.
//

#ifndef GAMEENGINE_COLOR_H
#define GAMEENGINE_COLOR_H
#include <string>
#include <SDL3/SDL.h>

class Color {
public:
    Color(int r, int g, int b);
    Color(const std::string& hex);

    std::string toHex() const;
    void setFromHex(const std::string& hex);

    SDL_Color toSdlColor() const;

    int getR() const;
    void setR(int r);

    int getG() const;
    void setG(int g);

    int getB() const;
    void setB(int b);

    int getA() const;
    void setA(int a);

private:
    int _r;
    int _g;
    int _b;
    int _a;
};

#endif //GAMEENGINE_COLOR_H