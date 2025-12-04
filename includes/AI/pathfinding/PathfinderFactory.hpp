//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_PATHFINDINGFACTORY_HPP
#define GAMEENGINE_PATHFINDINGFACTORY_HPP
#include "IPathfinder.hpp"

class PathfinderFactory {
public:
    static std::unique_ptr<IPathfinder> getPathfinder(std::string type = "AStar");
};

#endif //GAMEENGINE_PATHFINDINGFACTORY_HPP