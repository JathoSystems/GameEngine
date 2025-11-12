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
    }
}

bool Font::load() {
    _font = TTF_OpenFont(_path.c_str(), _size);
    return _font;
}

TTF_Font* Font::getSdlFont() {
    if (!_font) load();

    return _font;
}

void Font::setSize(int size) {
    _size = size;
    load();
}
