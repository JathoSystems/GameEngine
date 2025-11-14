//
// Created by jusra on 11-11-2025.
//
#include "UI/Font.h"
#include <iostream>

Font::Font(const std::string& path, const std::string& name)
    : _path(path), _name(name) {
    load();
}

Font::~Font() {
    cleanup();
}

void Font::cleanup() {
    if (_font) {
        TTF_CloseFont(_font);
        _font = nullptr;
    }
}

bool Font::load() {
    cleanup(); // Sluit eerst het oude font!

    _font = TTF_OpenFont(_path.c_str(), _size);
    if (!_font) {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

TTF_Font* Font::getSdlFont() {
    if (!_font) {
        load();
    }
    return _font;
}

void Font::setSize(int size) {
    if (_size != size) {
        _size = size;
        load();
    }
}

std::unique_ptr<Font> Font::getDefaultFont() {
    return std::make_unique<Font>("..\\resources\\fonts\\default.ttf", "test");
}
