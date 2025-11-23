#include "AI/pathfinding/AStarPathfinder.hpp"
#include "GameObjects/GameObject.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <unordered_map>

// Fast manhattan distance
static inline int manhattan(const Position& a, const Position& b) {
    return std::abs(a.getX() - b.getX()) + std::abs(a.getY() - b.getY());
}

// Node voor A* algoritme
struct Node {
    Position pos;
    int g;  // Cost from start
    int h;  // Heuristic to end
    int f;  // Total cost (g + h)
    Node* parent;

    Node(const Position& p, int gCost, int hCost, Node* par = nullptr)
        : pos(p), g(gCost), h(hCost), f(gCost + hCost), parent(par) {}

    // Voor priority queue (min-heap)
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

// Hash functie voor Position in unordered_set
struct PositionHash {
    size_t operator()(const Position& p) const {
        return std::hash<int>()(p.getX()) ^ (std::hash<int>()(p.getY()) << 1);
    }
};

// Equality voor Position
struct PositionEqual {
    bool operator()(const Position& a, const Position& b) const {
        return a.getX() == b.getX() && a.getY() == b.getY();
    }
};

// Check of een positie walkable is
static bool isWalkable(Scene* scene, const Position& pos, int cellSize) {
    return true;
    if (!scene) {
        std::cout << "  [isWalkable] Scene is null!" << std::endl;
        return false;
    }

    // Haal alle GameObjects op de scene
    const auto& objects = scene->getObjects();

    std::cout << "  [isWalkable] Checking grid (" << pos.getX() << ", " << pos.getY()
              << ") -> world (" << pos.getX() * cellSize << ", " << pos.getY() * cellSize
              << ") against " << objects.size() << " objects" << std::endl;

    for (const auto& obj : objects) {
        if (!obj) continue;

        // if (!obj->isSolid()) {
            // std::cout << "    Object at (" << obj->getPosition().getX() << ", "
                      // << obj->getPosition().getY() << ") is NOT solid, skipping" << std::endl;
            // continue;
        // }

        const Position& objPos = *obj->getTransform()->getPosition();
        int objX = objPos.getX();
        int objY = objPos.getY();
        int objWidth = obj->getTransform()->getSize()->getWidth();
        int objHeight = obj->getTransform()->getSize()->getHeight();

        // Check overlap met grid cell
        int cellX = pos.getX() * cellSize;
        int cellY = pos.getY() * cellSize;

        std::cout << "    Checking solid object: pos(" << objX << ", " << objY
                  << ") size(" << objWidth << "x" << objHeight << ")" << std::endl;
        std::cout << "      Cell: (" << cellX << ", " << cellY
                  << ") to (" << cellX + cellSize << ", " << cellY + cellSize << ")" << std::endl;

        if (cellX < objX + objWidth &&
            cellX + cellSize > objX &&
            cellY < objY + objHeight &&
            cellY + cellSize > objY) {
            std::cout << "      COLLISION! Cell is blocked" << std::endl;
            return false;
        }
    }

    std::cout << "  [isWalkable] Cell is WALKABLE" << std::endl;
    return true;
}

std::vector<std::unique_ptr<Position>> AStarPathfinder::getPath(Scene* scene,
                                                                const Position& start,
                                                                const Position& end,
                                                                int cellSize) {
    std::vector<std::unique_ptr<Position>> path;

    std::cout << "\n=== A* Pathfinding Debug ===" << std::endl;
    std::cout << "Start: (" << start.getX() << ", " << start.getY() << ")" << std::endl;
    std::cout << "End: (" << end.getX() << ", " << end.getY() << ")" << std::endl;
    std::cout << "Cell size: " << cellSize << std::endl;

    if (!scene) {
        std::cout << "ERROR: Scene is null!" << std::endl;
        return path;
    }

    if (cellSize <= 0) {
        std::cout << "ERROR: Invalid cell size: " << cellSize << std::endl;
        return path;
    }

    // Converteer world coordinates naar grid coordinates
    Position gridStart(start.getX() / cellSize, start.getY() / cellSize);
    Position gridEnd(end.getX() / cellSize, end.getY() / cellSize);

    std::cout << "Grid start: (" << gridStart.getX() << ", " << gridStart.getY() << ")" << std::endl;
    std::cout << "Grid end: (" << gridEnd.getX() << ", " << gridEnd.getY() << ")" << std::endl;

    // Check of start walkable is
    std::cout << "\nChecking START walkability:" << std::endl;
    bool startWalkable = isWalkable(scene, gridStart, cellSize);

    // Check of end walkable is
    std::cout << "\nChecking END walkability:" << std::endl;
    bool endWalkable = isWalkable(scene, gridEnd, cellSize);

    if (!startWalkable) {
        std::cout << "\nERROR: Start position is not walkable!" << std::endl;
        return path;
    }

    if (!endWalkable) {
        std::cout << "\nERROR: End position is not walkable!" << std::endl;
        return path;
    }

    // Als start == end, return direct
    if (gridStart.getX() == gridEnd.getX() && gridStart.getY() == gridEnd.getY()) {
        std::cout << "Start == End, returning direct path" << std::endl;
        path.push_back(std::make_unique<Position>(end));
        return path;
    }

    std::cout << "\nStarting A* search..." << std::endl;

    // Priority queue voor open set (min-heap op f-waarde)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

    // Closed set voor bezochte nodes
    std::unordered_set<Position, PositionHash, PositionEqual> closedSet;

    // Map voor node tracking (voor parent pointers)
    std::unordered_map<Position, std::unique_ptr<Node>, PositionHash, PositionEqual> nodeMap;

    // Start node toevoegen
    int h = manhattan(gridStart, gridEnd);
    auto startNode = std::make_unique<Node>(gridStart, 0, h);
    Node* startPtr = startNode.get();
    nodeMap[gridStart] = std::move(startNode);
    openSet.push(*startPtr);

    // 4-richting beweging (up, down, left, right)
    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    Node* endNode = nullptr;
    int iterations = 0;
    int maxIterations = 10000; // Safety limit

    // A* main loop
    while (!openSet.empty() && iterations < maxIterations) {
        iterations++;

        Node current = openSet.top();
        openSet.pop();

        Position currentPos = current.pos;

        if (iterations % 100 == 0) {
            std::cout << "Iteration " << iterations << ": exploring ("
                      << currentPos.getX() << ", " << currentPos.getY()
                      << ") f=" << current.f << std::endl;
        }

        // Check of we al in closed set zitten
        if (closedSet.count(currentPos)) {
            continue;
        }

        closedSet.insert(currentPos);

        // Check of we het einde bereikt hebben
        if (currentPos.getX() == gridEnd.getX() && currentPos.getY() == gridEnd.getY()) {
            std::cout << "PATH FOUND after " << iterations << " iterations!" << std::endl;
            endNode = nodeMap[currentPos].get();
            break;
        }

        // Exploreer buren
        for (int i = 0; i < 4; i++) {
            int newX = currentPos.getX() + dx[i];
            int newY = currentPos.getY() + dy[i];
            Position neighbor(newX, newY);

            // Check of neighbor al bezocht is
            if (closedSet.count(neighbor)) {
                continue;
            }

            // Check of neighbor walkable is
            if (iterations < 10) { // Alleen eerste paar iteraties debuggen
                std::cout << "  Checking neighbor (" << newX << ", " << newY << "):" << std::endl;
            }

            if (!isWalkable(scene, neighbor, cellSize)) {
                if (iterations < 10) {
                    std::cout << "    Neighbor blocked!" << std::endl;
                }
                continue;
            }

            int newG = current.g + 1;  // Cost is 1 per stap
            int newH = manhattan(neighbor, gridEnd);

            // Check of we deze node al hebben met een betere cost
            auto it = nodeMap.find(neighbor);
            if (it != nodeMap.end() && it->second->g <= newG) {
                continue;
            }

            // Nieuwe node toevoegen of updaten
            Node* currentNodePtr = nodeMap[currentPos].get();
            auto newNode = std::make_unique<Node>(neighbor, newG, newH, currentNodePtr);
            Node* newNodePtr = newNode.get();
            nodeMap[neighbor] = std::move(newNode);
            openSet.push(*newNodePtr);
        }
    }

    std::cout << "Search completed after " << iterations << " iterations" << std::endl;
    std::cout << "Closed set size: " << closedSet.size() << std::endl;

    // Als we geen pad gevonden hebben
    if (!endNode) {
        std::cout << "NO PATH FOUND!" << std::endl;
        return path;
    }

    // Reconstrueer pad van end naar start
    std::vector<Position> reversePath;
    Node* current = endNode;
    while (current != nullptr) {
        reversePath.push_back(current->pos);
        current = current->parent;
    }

    std::cout << "Path length: " << reversePath.size() << " waypoints" << std::endl;

    // Converteer terug naar world coordinates en reverse de volgorde
    for (auto it = reversePath.rbegin(); it != reversePath.rend(); ++it) {
        int worldX = it->getX() * cellSize + cellSize / 2;
        int worldY = it->getY() * cellSize + cellSize / 2;
        path.push_back(std::make_unique<Position>(worldX, worldY));
    }

    std::cout << "=== End A* Debug ===\n" << std::endl;

    return path;
}