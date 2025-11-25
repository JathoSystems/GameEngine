//
// Created by jusra on 14-11-2025.
//
#include <cmath>

#include "Animation/Converter/ElesticConverter.hpp"

int ElasticConverter::convert(int start, int end, float t) {
    if (t == 0.0f) return start;
    if (t == 1.0f) return end;

    const float c4 = (2.0f * 3.14) / 3.0f;

    float v = std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;

    float out = static_cast<float>(start) + (static_cast<float>(end) - static_cast<float>(start)) * v;
    return static_cast<int>(std::lround(out));
}