//
// Created by jusra on 21-11-2025.
//

#ifndef GAMEENGINE_IPATHFINDER_HPP
#define GAMEENGINE_IPATHFINDER_HPP
#include <vector>

#include "GameObjects/Transform/Position.h"
#include "Scenes/Scene.h"

class IPathfinder {
public:
    virtual ~IPathfinder() = default;

    // interval is de afstand tussen de punten in de gegenereerde pad (standaard 5 pixels)
    virtual std::vector<std::unique_ptr<Position>> getPath(Scene* scene, const Position& start, const Position& end, int interval = 5) = 0;
};

#endif //GAMEENGINE_IPATHFINDER_HPP