#ifndef GAMEENGINE_ASTARPATHFINDER_HPP
#define GAMEENGINE_ASTARPATHFINDER_HPP
#include "IPathfinder.hpp"

class AStarPathfinder : public IPathfinder {
public:
    std::vector<std::unique_ptr<Position> > getPath(Scene *scene, const Position &start, const Position &end,
                                                    int interval, std::string heuristicType) override;
};

#endif //GAMEENGINE_ASTARPATHFINDER_HPP
