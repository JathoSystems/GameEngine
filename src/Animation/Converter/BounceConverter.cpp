//
// Created by jusra on 14-11-2025.
//
#include "Animation/Converter/BounceConverter.hpp"

#include <algorithm>
#include <cmath>

int BounceConverter::convert(int start, int end, float t) {
    // Clamp t
    t = std::clamp(t, 0.0f, 1.0f);

    const float n1 = 7.5625f;
    const float d1 = 2.75f;
    float v;

    if (t < 1.0f / d1) {
        v = n1 * t * t;
    } else if (t < 2.0f / d1) {
        t -= 1.5f / d1;
        v = n1 * t * t + 0.75f;
    } else if (t < 2.5f / d1) {
        t -= 2.25f / d1;
        v = n1 * t * t + 0.9375f;
    } else {
        t -= 2.625f / d1;
        v = n1 * t * t + 0.984375f;
    }

    // Map v (0..1) naar start..end en rond netjes af
    float out = static_cast<float>(start) + (static_cast<float>(end) - static_cast<float>(start)) * v;
    return static_cast<int>(std::lround(out)); // of use floor/cast als je dat wil
}
