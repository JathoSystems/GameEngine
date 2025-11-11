//
// Created by jusra on 11-11-2025.
//

#ifndef GAMEENGINE_FONT_H
#define GAMEENGINE_FONT_H
#include <string>

#include "SDL3_ttf/SDL_ttf.h"

class Font {
private:
    std::string _path;
    std::string _name;
    TTF_Font *_font;
    int _size = 16;

public:
    Font(const std::string& path, const std::string& name);
    ~Font();
    bool load();

    TTF_Font * getSdlFont();
};

#endif //GAMEENGINE_FONT_H