//
// Created by jusra on 11-11-2025.
//
#include "UI/Color.h"

#include <iomanip>
#include <sstream>

Color::Color(int r, int g, int b)
    : _r(r), _g(g), _b(b), _a(255) {}

Color::Color(const std::string& hex)
    : _r(0), _g(0), _b(0), _a(255) {
    setFromHex(hex);
}

std::string Color::toHex() const {
    std::ostringstream ss;
    ss << "#"
       << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(_r) & 0xff)
       << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(_g) & 0xff)
       << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(_b) & 0xff);
    return ss.str();
}

void Color::setFromHex(const std::string& hex) {
    std::string h = hex;
    if (!h.empty() && h[0] == '#') h = h.substr(1);

    if (h.size() != 6) return;

    _r = std::stoi(h.substr(0, 2), nullptr, 16);
    _g = std::stoi(h.substr(2, 2), nullptr, 16);
    _b = std::stoi(h.substr(4, 2), nullptr, 16);
}

SDL_Color Color::toSdlColor() const {
    return SDL_Color{
        static_cast<Uint8>(_r),
        static_cast<Uint8>(_g),
        static_cast<Uint8>(_b),
        static_cast<Uint8>(_a)
    };
}

void Color::setR(int r) {
    _r = r;
}

int Color::getG() const {
    return _g;
}

void Color::setG(int g) {
    _g = g;
}

int Color::getB() const {
    return _b;
}

void Color::setB(int b) {
    _b = b;
}

int Color::getA() const {
    return _a;
}

void Color::setA(int a) {
    _a = a;
}

int Color::getR() const {
    return _r;
}