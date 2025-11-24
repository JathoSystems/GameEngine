//
// Created by jusra on 24-11-2025.
//

#include <iostream>

#include "AI/pathfinding/heuristics/HeuristicCalculatorFactory.hpp"
#include "AI/pathfinding/heuristics/EuclideanHeuristicCalculator.hpp"
#include "AI/pathfinding/heuristics/ManhattenHeuristicCalculator.hpp"

std::unique_ptr<IHeuristicCalculator> HeuristicCalculatorFactory::getHeuristicCalculator(std::string type) {
    if (type == "manhatten") return std::make_unique<ManhattenHeuristicCalculator>();
    if (type == "euclidean") return std::make_unique<EuclideanHeuristicCalculator>();

    std::cerr << "WARNING: HeuristicCalculatorFactory::getHeuristicCalculator unknown type '"
            << type << "', defaulting to 'manhatten'" << std::endl;
    return std::make_unique<ManhattenHeuristicCalculator>();
}
