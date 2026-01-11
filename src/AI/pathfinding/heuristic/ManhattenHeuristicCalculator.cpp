#include "AI/pathfinding/heuristics/ManhattenHeuristicCalculator.hpp"
#include "AI/pathfinding/heuristics/IHeuristicCalculator.hpp"

int ManhattenHeuristicCalculator::calculateHeuristic(const Position &a, const Position &b) {
    int dx = std::abs(a.getX() - b.getX());
    int dy = std::abs(a.getY() - b.getY());

    int manhattan = dx + dy;
    int cross = std::abs(dx - dy);
    return manhattan * 10 + cross;
}
