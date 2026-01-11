#include "Animation/Animation.hpp"
#include "Animation/Converter/ConverterFactory.hpp"
#include "Animation/Keyframe.hpp"
#include "SDL/Window.h"
#include <cmath>
#include <iostream>

float Animation::getMaxTime() const {
    float currentLargest = 0.0f;
    for (const auto &pair: _keyFrames) {
        if (pair.first > currentLargest) {
            currentLargest = pair.first;
        }
    }
    return currentLargest;
}

Animation::Animation(AnimationType type) {
    _converter = ConverterFactory::createConverter(type);
}

void Animation::addKeyframe(float time, std::unique_ptr<Keyframe> frame) {
    _keyFrames.emplace(time, std::move(frame));
}

void Animation::update(float deltaTime) {
    if (_keyFrames.empty()) return;

    _time += deltaTime;

    float maxTime = getMaxTime();
    if (_time > maxTime) {
        _time = fmod(_time, maxTime);
    }
}

void Animation::render(const std::unique_ptr<Window> &window) {
    if (_keyFrames.empty()) return;

    float maxTime = getMaxTime();
    float animTime = fmod(_time, maxTime);

    Keyframe *current = nullptr;
    Keyframe *next = nullptr;
    float currentTime = 0.0f;
    float nextTime = 0.0f;

    auto it = _keyFrames.lower_bound(animTime);

    if (it != _keyFrames.end() && it->first == animTime) {
        current = it->second.get();
        next = current;
        currentTime = nextTime = animTime;
    } else {
        if (it != _keyFrames.end()) {
            next = it->second.get();
            nextTime = it->first;
        } else {
            next = _keyFrames.begin()->second.get();
            nextTime = _keyFrames.begin()->first;
        }

        if (it != _keyFrames.begin()) {
            auto prev = std::prev(it);
            current = prev->second.get();
            currentTime = prev->first;
        } else {
            auto last = std::prev(_keyFrames.end());
            current = last->second.get();
            currentTime = last->first;
        }
    }

    float t = 0.0f;
    float dt = nextTime - currentTime;

    if (dt < 0.0f) {
        dt += maxTime;
    }

    if (dt != 0.0f) {
        float raw = animTime - currentTime;
        if (raw < 0.0f) raw += maxTime;

        t = raw / dt;
    }
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    if (current && next && _converter) {
        current->transition(
            _converter.get(),
            current->getTransform(),
            next->getTransform(),
            _parent,
            t
        );
    }
}
