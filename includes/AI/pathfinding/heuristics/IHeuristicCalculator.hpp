//
// Created by jusra on 24-11-2025.
//

#ifndef GAMEENGINE_IHEURISTICCALCULATOR_HPP
#define GAMEENGINE_IHEURISTICCALCULATOR_HPP
#include "GameObjects/Transform/Position.h"

class IHeuristicCalculator {
public:
    virtual ~IHeuristicCalculator() = default;

    virtual int calculateHeuristic(const Position &a, const Position &b) = 0;
};

#endif //GAMEENGINE_IHEURISTICCALCULATOR_HPP