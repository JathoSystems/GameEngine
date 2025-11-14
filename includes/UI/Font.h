//
// Created by jusra on 11-11-2025.
//

#ifndef GAMEENGINE_FONT_H
#define GAMEENGINE_FONT_H
#include <memory>
#include <string>

#include "SDL3_ttf/SDL_ttf.h"

class Font {
private:
    std::string _path;
    std::string _name;
    TTF_Font* _font = nullptr;
    float _size = 24;

    void cleanup();

public:
    Font(const std::string& path, const std::string& name);
    ~Font();

    bool load();
    TTF_Font* getSdlFont();
    void setSize(int size);
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    static std::unique_ptr<Font> getDefaultFont();
};

#endif //GAMEENGINE_FONT_H