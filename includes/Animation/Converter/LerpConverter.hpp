#ifndef GAMEENGINE_LERPCONVERTER_HPP
#define GAMEENGINE_LERPCONVERTER_HPP
#include "IConverter.hpp"

class LerpConverter : public IConverter {
public:
    int convert(int start, int end, float t) override;
};

#endif //GAMEENGINE_LERPCONVERTER_HPP
