#include "AI/pathfinding/heuristics/EuclideanHeuristicCalculator.hpp"
#include <cmath>

int EuclideanHeuristicCalculator::calculateHeuristic(const Position &a, const Position &b) {
    int dx = a.getX() - b.getX();
    int dy = a.getY() - b.getY();
    return static_cast<int>(std::sqrt(dx * dx + dy * dy) * 10);
}
