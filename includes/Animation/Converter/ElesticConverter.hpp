//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_ELESTICCONVERTER_HPP
#define GAMEENGINE_ELESTICCONVERTER_HPP
#include "IConverter.hpp"

class ElasticConverter : public IConverter {
public:
    int convert(int start, int end, float t) override;
};

#endif //GAMEENGINE_ELESTICCONVERTER_HPP