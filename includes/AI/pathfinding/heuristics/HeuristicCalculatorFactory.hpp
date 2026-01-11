#ifndef GAMEENGINE_HEURISTICCALCULATORFACTORY_HPP
#define GAMEENGINE_HEURISTICCALCULATORFACTORY_HPP
#include <memory>
#include "IHeuristicCalculator.hpp"

class HeuristicCalculatorFactory {
public:
    static std::unique_ptr<IHeuristicCalculator> getHeuristicCalculator(std::string type = "manhatten");
};

#endif //GAMEENGINE_HEURISTICCALCULATORFACTORY_HPP
