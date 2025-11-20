//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_KEYFRAME_HPP
#define GAMEENGINE_KEYFRAME_HPP
#include "Converter/IConverter.hpp"
#include "GameObjects/GameObject.h"
#include "GameObjects/Transform/Transform.h"

class Keyframe {
private:
    std::unique_ptr<Transform> _target;

public:
    Keyframe(std::unique_ptr<Transform> transform);
    void transition(IConverter *converter, Transform *prev, Transform* target, GameObject *parent, float time);

    Transform* getTransform();
};

#endif //GAMEENGINE_KEYFRAME_HPP
