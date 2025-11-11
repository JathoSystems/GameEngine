//
// Created by jusra on 11-11-2025.
//
#include "UI/Font.h"
#include <iostream>

Font::Font(const std::string& path, const std::string& name)
    : _path(path), _name(name) {}

Font::~Font() {
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
        std::cout << "[Font] Closed font: " << _name << std::endl;
    }
}

bool Font::load() {
    _font = TTF_OpenFont(_path.c_str(), _size);
    if (!_font) {
        std::cerr << "[Font] Failed to open font from " << _path
                  << ": " << SDL_GetError() << std::endl;
        return false;
    }


    std::cout << "[Font] Loaded font: " << _name
              << " (" << _path << ", size " << _size << ")" << std::endl;
    return true;
}

TTF_Font* Font::getSdlFont() {
    if (!_font) load();


    return _font;
}
