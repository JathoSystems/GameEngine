#include "AI/pathfinding/AStarPathfinder.hpp"
#include "GameObjects/GameObject.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <queue>

// --- Optimized Node struct ---
struct Node {
    Position pos;
    int gCost;
    int hCost;
    int fCost;
    Node* parent;

    Node(const Position& p, int g, int h, Node* par = nullptr)
        : pos(p), gCost(g), hCost(h), fCost(g + h), parent(par) {}
};

// Priority queue comparator
struct NodeCmp {
    bool operator()(Node* a, Node* b) const {
        return (a->fCost > b->fCost) || (a->fCost == b->fCost && a->hCost > b->hCost);
    }
};

// Position hash for unordered_set
struct PosHash {
    std::size_t operator()(const Position& p) const {
        return std::hash<int>()(p.getX()) ^ (std::hash<int>()(p.getY()) << 1);
    }
};

struct PosEqual {
    bool operator()(const Position& a, const Position& b) const {
        return a.getX() == b.getX() && a.getY() == b.getY();
    }
};

// Fast manhattan distance
static inline int manhattan(const Position& a, const Position& b) {
    return std::abs(a.getX() - b.getX()) + std::abs(a.getY() - b.getY());
}

// Build spatial grid for faster collision detection
struct SpatialGrid {
    std::vector<std::vector<bool>> blocked;
    int width, height, cellSize;

    SpatialGrid(Scene* scene, int w, int h, int cs) : width(w), height(h), cellSize(cs) {
        blocked.resize(h, std::vector<bool>(w, false));

        for(auto& obj : scene->getObjects()) {
            Position* objPos = obj->getTransform()->getPosition();
            Size* objSize = obj->getTransform()->getSize();

            int ox = objPos->getX();
            int oy = objPos->getY();
            int ow = objSize->getWidth();
            int oh = objSize->getHeight();

            // Calculate grid cell range that could overlap with this object
            // Grid position (gx, gy) corresponds to pixel position (gx * cellSize, gy * cellSize)
            int minGx = ox / cellSize;
            int maxGx = (ox + ow + cellSize - 1) / cellSize;
            int minGy = oy / cellSize;
            int maxGy = (oy + oh + cellSize - 1) / cellSize;

            // Clamp to grid bounds
            minGx = std::max(0, minGx);
            maxGx = std::min(width, maxGx);
            minGy = std::max(0, minGy);
            maxGy = std::min(height, maxGy);

            // Mark cells that overlap with the object
            for(int gy = minGy; gy < maxGy; ++gy) {
                for(int gx = minGx; gx < maxGx; ++gx) {
                    int px = gx * cellSize;
                    int py = gy * cellSize;

                    // Check if this grid cell (which represents a point at px,py)
                    // overlaps with the object bounds
                    bool overlaps = px < ox + ow &&
                                   px + cellSize > ox &&
                                   py < oy + oh &&
                                   py + cellSize > oy;

                    if(overlaps) {
                        blocked[gy][gx] = true;
                    }
                }
            }
        }
    }

    inline bool isBlocked(int x, int y) const {
        return x < 0 || x >= width || y < 0 || y >= height || blocked[y][x];
    }
};

// Calculate maze bounds
static void getMazeBounds(Scene* scene, int& maxX, int& maxY, int cellSize) {
    maxX = 0;
    maxY = 0;

    for(auto& obj : scene->getObjects()) {
        int gridX = obj->getTransform()->getPosition()->getX() / cellSize;
        int gridY = obj->getTransform()->getPosition()->getY() / cellSize;
        maxX = std::max(maxX, gridX);
        maxY = std::max(maxY, gridY);
    }

    maxX += 2;
    maxY += 2;
}

// --- Optimized A* implementation ---
std::vector<std::unique_ptr<Position>> AStarPathfinder::getPath(Scene* scene,
                                                                const Position& start,
                                                                const Position& end,
                                                                int cellSize) {
    int mazeWidth, mazeHeight;
    getMazeBounds(scene, mazeWidth, mazeHeight, cellSize);

    // Validate bounds
    if(start.getX() < 0 || start.getX() >= mazeWidth ||
       start.getY() < 0 || start.getY() >= mazeHeight ||
       end.getX() < 0 || end.getX() >= mazeWidth ||
       end.getY() < 0 || end.getY() >= mazeHeight) {
        return {};
    }

    // Build spatial grid once
    SpatialGrid grid(scene, mazeWidth, mazeHeight, cellSize);

    if(grid.isBlocked(start.getX(), start.getY()) ||
       grid.isBlocked(end.getX(), end.getY())) {
        return {};
    }

    // Use priority queue instead of linear search
    std::priority_queue<Node*, std::vector<Node*>, NodeCmp> openList;
    std::unordered_set<Position, PosHash, PosEqual> closedSet;
    std::vector<Node*> allNodes;
    allNodes.reserve(mazeWidth * mazeHeight / 4); // Reserve reasonable size

    Node* startNode = new Node(start, 0, manhattan(start, end));
    openList.push(startNode);
    allNodes.push_back(startNode);

    // 2D grid for fast node lookup
    std::vector<std::vector<Node*>> nodeGrid(mazeHeight, std::vector<Node*>(mazeWidth, nullptr));
    nodeGrid[start.getY()][start.getX()] = startNode;

    static constexpr int dx[4] = {1, -1, 0, 0};
    static constexpr int dy[4] = {0, 0, 1, -1};

    while(!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        // Skip if already processed
        if(closedSet.count(current->pos)) continue;
        closedSet.insert(current->pos);

        // Goal reached
        if(current->pos.getX() == end.getX() && current->pos.getY() == end.getY()) {
            std::vector<std::unique_ptr<Position>> path;
            path.reserve(current->gCost + 1);

            for(Node* node = current; node != nullptr; node = node->parent) {
                path.push_back(std::make_unique<Position>(node->pos));
            }

            std::reverse(path.begin(), path.end());

            // Cleanup
            for(auto n : allNodes) delete n;
            return path;
        }

        // Explore neighbors
        for(int i = 0; i < 4; ++i) {
            int nx = current->pos.getX() + dx[i];
            int ny = current->pos.getY() + dy[i];

            if(grid.isBlocked(nx, ny)) continue;

            Position neighborPos(nx, ny);
            if(closedSet.count(neighborPos)) continue;

            int tentativeG = current->gCost + 1;
            Node* neighborNode = nodeGrid[ny][nx];

            if(!neighborNode) {
                neighborNode = new Node(neighborPos, tentativeG, manhattan(neighborPos, end), current);
                allNodes.push_back(neighborNode);
                nodeGrid[ny][nx] = neighborNode;
                openList.push(neighborNode);
            } else if(tentativeG < neighborNode->gCost) {
                neighborNode->gCost = tentativeG;
                neighborNode->fCost = tentativeG + neighborNode->hCost;
                neighborNode->parent = current;
                openList.push(neighborNode);
            }
        }
    }

    // No path found - cleanup
    for(auto n : allNodes) delete n;
    return {};
}