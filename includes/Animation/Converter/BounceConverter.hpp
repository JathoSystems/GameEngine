//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_BOUNCECONVERTER_HPP
#define GAMEENGINE_BOUNCECONVERTER_HPP
#include "IConverter.hpp"

class BounceConverter : public IConverter {
public:
    int convert(int start, int end, float t) override;
};

#endif //GAMEENGINE_BOUNCECONVERTER_HPP