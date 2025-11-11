//
// Created by jusra on 11-11-2025.
//
#include "UI/Color.h"

#include <iomanip>

Color::Color(int r, int g, int b) {
    _r = r;
    _g = g;
    _b = b;
}

Color::Color(const std::string& hex) {
    setFromHex(hex);
}

std::string Color::toHex() {
    std::ostringstream ss;
    ss << "#"
       << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(_r)
       << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(_g)
       << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(_b);
    return ss.str();
}

void Color::setFromHex(const std::string& hex) {
    std::string h = hex;
    if (h[0] == '#') h = h.substr(1);

    if (h.size() != 6) return;

    _r = std::stoi(h.substr(0, 2), nullptr, 16);
    _g = std::stoi(h.substr(2, 2), nullptr, 16);
    _b = std::stoi(h.substr(4, 2), nullptr, 16);
}

SDL_Color Color::toSdlColor() {
    return SDL_Color{_r, _g, _b, 255};
}
