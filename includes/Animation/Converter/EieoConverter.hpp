//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_EIEOCONVERTER_HPP
#define GAMEENGINE_EIEOCONVERTER_HPP
#include "IConverter.hpp"

class EieoConverter : public IConverter {
public:
    int convert(int start, int end, float t) override;
};

#endif //GAMEENGINE_EIEOCONVERTER_HPP