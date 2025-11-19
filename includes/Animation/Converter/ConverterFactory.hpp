//
// Created by jusra on 14-11-2025.
//

#ifndef GAMEENGINE_CONVERTERFACTORY_HPP
#define GAMEENGINE_CONVERTERFACTORY_HPP
#include "IConverter.hpp"
#include "Animation/Animation.hpp"
#include <memory>

class ConverterFactory  {
public:
    static std::unique_ptr<IConverter> createConverter(AnimationType type);
};

#endif //GAMEENGINE_CONVERTERFACTORY_HPP