#ifndef GAMEENGINE_MANHATTENHEURISTICCALCULATOR_HPP
#define GAMEENGINE_MANHATTENHEURISTICCALCULATOR_HPP
#include "IHeuristicCalculator.hpp"

class ManhattenHeuristicCalculator : public IHeuristicCalculator {
public:
    int calculateHeuristic(const Position &a, const Position &b) override;
};

#endif //GAMEENGINE_MANHATTENHEURISTICCALCULATOR_HPP
