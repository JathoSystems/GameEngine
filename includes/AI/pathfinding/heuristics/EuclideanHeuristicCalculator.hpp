#ifndef GAMEENGINE_EUCLIDEANHEURISTICCALCULATOR_HPP
#define GAMEENGINE_EUCLIDEANHEURISTICCALCULATOR_HPP
#include "IHeuristicCalculator.hpp"

class EuclideanHeuristicCalculator : public IHeuristicCalculator {
public:
    int calculateHeuristic(const Position &a, const Position &b) override;
};

#endif //GAMEENGINE_EUCLIDEANHEURISTICCALCULATOR_HPP
