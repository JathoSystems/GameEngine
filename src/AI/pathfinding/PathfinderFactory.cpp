#include "AI/pathfinding/PathfinderFactory.hpp"

#include "AI/pathfinding/AStarPathfinder.hpp"

std::unique_ptr<IPathfinder> PathfinderFactory::getPathfinder(std::string type) {
    return std::make_unique<AStarPathfinder>();
}
