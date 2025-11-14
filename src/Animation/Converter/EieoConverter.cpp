//
// Created by jusra on 14-11-2025.
//
#include "Animation/Converter/EieoConverter.hpp"

int EieoConverter::convert(int start, int end, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // Ease In Ease Out (smoothstep)
    float ease = t * t * (3.0f - 2.0f * t);

    return static_cast<int>(start + (end - start) * ease);
}