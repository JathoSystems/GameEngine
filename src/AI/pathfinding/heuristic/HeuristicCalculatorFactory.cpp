#include <iostream>
#include "AI/pathfinding/heuristics/HeuristicCalculatorFactory.hpp"
#include "AI/pathfinding/heuristics/EuclideanHeuristicCalculator.hpp"
#include "AI/pathfinding/heuristics/ManhattenHeuristicCalculator.hpp"

std::unique_ptr<IHeuristicCalculator> HeuristicCalculatorFactory::getHeuristicCalculator(std::string type) {
    if (type == "manhattan") return std::make_unique<ManhattenHeuristicCalculator>();
    if (type == "euclidean") return std::make_unique<EuclideanHeuristicCalculator>();

    std::cerr << "WARNING: HeuristicCalculatorFactory::getHeuristicCalculator unknown type '"
            << type << "', defaulting to 'manhattan'" << std::endl;
    return std::make_unique<ManhattenHeuristicCalculator>();
}
