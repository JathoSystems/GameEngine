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

#include "AI/pathfinding/heuristics/HeuristicCalculatorFactory.hpp"
#include "AI/pathfinding/heuristics/IHeuristicCalculator.hpp"

struct Node {
    Position pos;
    int cost;
    int heuristic;
    int estimatedCost;
    Node *parent;

    Node(const Position &p, int gCost, int hCost, Node *par = nullptr)
        : pos(p), cost(gCost), heuristic(hCost), estimatedCost(gCost + hCost), parent(par) {
    }

    bool operator>(const Node &other) const {
        return estimatedCost > other.estimatedCost;
    }
};

static bool isWalkable(Scene *scene, const Position &pos, int cellSize) {
    if (!scene) {
        return false;
    }

    // Haal alle GameObjects op de scene
    const auto &objects = scene->getObjects();

    for (const auto &obj: objects) {
        if (!obj) continue;

        // if (!obj->isSolid()) {
        // std::cout << "    Object at (" << obj->getPosition().getX() << ", "
        // << obj->getPosition().getY() << ") is NOT solid, skipping" << std::endl;
        // continue;
        // }

        const Position &objPos = *obj->getTransform()->getPosition();
        int objX = objPos.getX();
        int objY = objPos.getY();
        int objWidth = obj->getTransform()->getSize()->getWidth();
        int objHeight = obj->getTransform()->getSize()->getHeight();

        int cellX = pos.getX() * cellSize;
        int cellY = pos.getY() * cellSize;

        if (cellX < objX + objWidth &&
            cellX + cellSize > objX &&
            cellY < objY + objHeight &&
            cellY + cellSize > objY) {
            return false;
        }
    }
    return true;
}

std::vector<std::unique_ptr<Position> > AStarPathfinder::getPath(Scene *scene,
                                                                 const Position &start,
                                                                 const Position &end,
                                                                 int cellSize,
                                                                 std::string heuristicType) {
    std::vector<std::unique_ptr<Position> > path;
    std::unique_ptr<IHeuristicCalculator> heuristicCalculator = HeuristicCalculatorFactory::getHeuristicCalculator(heuristicType);
    if (!scene) {
        return path;
    }

    if (cellSize <= 0) {
        return path;
    }

    Position gridStart(start.getX() / cellSize, start.getY() / cellSize);
    Position gridEnd(end.getX() / cellSize, end.getY() / cellSize);

    bool startWalkable = isWalkable(scene, gridStart, cellSize);
    bool endWalkable = isWalkable(scene, gridEnd, cellSize);

    if (!startWalkable) {
        std::cout << "\nERROR: Start position is not walkable!" << std::endl;
        return path;
    }

    if (!endWalkable) {
        std::cout << "\nERROR: End position is not walkable!" << std::endl;
        return path;
    }

    if (gridStart.getX() == gridEnd.getX() && gridStart.getY() == gridEnd.getY()) {
        path.push_back(std::make_unique<Position>(end));
        return path;
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node> > openSet;
    std::unordered_set<Position> closedSet;
    std::unordered_map<Position, std::unique_ptr<Node>> nodeMap;

    int heuristic = heuristicCalculator->calculateHeuristic(gridStart, gridEnd);
    auto startNode = std::make_unique<Node>(gridStart, 0, heuristic);
    Node *startPtr = startNode.get();
    nodeMap[gridStart] = std::move(startNode);
    openSet.push(*startPtr);

    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};

    Node *endNode = nullptr;
    int iterations = 0;
    int maxIterations = 10000;

    while (!openSet.empty() && iterations < maxIterations) {
        iterations++;

        Node current = openSet.top();
        openSet.pop();

        Position currentPos = current.pos;

        if (closedSet.count(currentPos)) {
            continue;
        }

        closedSet.insert(currentPos);

        if (currentPos.getX() == gridEnd.getX() && currentPos.getY() == gridEnd.getY()) {
            endNode = nodeMap[currentPos].get();
            break;
        }

        for (int i = 0; i < 4; i++) {
            int newX = currentPos.getX() + dx[i];
            int newY = currentPos.getY() + dy[i];
            Position neighbor(newX, newY);

            if (closedSet.count(neighbor)) {
                continue;
            }

            if (!isWalkable(scene, neighbor, cellSize)) {
                continue;
            }

            int newG = current.cost + 1;
            int newH =  heuristicCalculator->calculateHeuristic(neighbor, gridEnd);

            auto it = nodeMap.find(neighbor);
            if (it != nodeMap.end() && it->second->cost <= newG) {
                continue;
            }

            Node *currentNodePtr = nodeMap[currentPos].get();
            auto newNode = std::make_unique<Node>(neighbor, newG, newH, currentNodePtr);
            Node *newNodePtr = newNode.get();
            nodeMap[neighbor] = std::move(newNode);
            openSet.push(*newNodePtr);
        }
    }

    if (!endNode) {
        std::cout << "NO PATH FOUND!" << std::endl;
        return path;
    }

    std::vector<Position> reversePath;
    Node *current = endNode;
    while (current != nullptr) {
        reversePath.push_back(current->pos);
        current = current->parent;
    }

    for (auto it = reversePath.rbegin(); it != reversePath.rend(); ++it) {
        int worldX = it->getX() * cellSize + cellSize / 2;
        int worldY = it->getY() * cellSize + cellSize / 2;
        path.push_back(std::make_unique<Position>(worldX, worldY));
    }

    return path;
}
