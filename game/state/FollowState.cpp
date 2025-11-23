#include "AI/pathfinding/PathfinderFactory.hpp"
#include "AI/states/IState.hpp"
#include <cmath>
#include <iostream>

class SimpleTransition : public ITransition {
private:
    int* x;
    int* y;
    int* targetX = new int(99999);
    int* targetY = new int(99999);

public:
    SimpleTransition(int* x, int* y, int* targetX, int* targetY)
        : x(x), y(y), targetX(targetX), targetY(targetY) {}
    bool shouldTransition() override {
        return *x == *targetX && *y == *targetY;
    }

    std::string getTargetState() override {
        return "Idle";
    }
};

class FollowState : public IState {
private:
    Scene* scene;
    GameObject* parent;
    int* targetX;
    int* targetY;
    int* currentX;
    int* currentY;
    float speed = 100.0f; // pixels per second

    // Float positie voor vloeiende beweging (geen afrondingsfouten!)
    float floatX = 0.0f;
    float floatY = 0.0f;
    bool floatPosInitialized = false;

    // Cache het pad om niet elke frame opnieuw te berekenen
    std::vector<std::unique_ptr<Position>> currentPath;
    size_t currentPathIndex = 0;
    int lastTargetX = -9999;
    int lastTargetY = -9999;

public:
    FollowState(Scene* scene, GameObject* object, int* currentX, int* currentY, int* x, int* y)
        : scene(scene), parent(object), targetX(x), targetY(y), currentX(currentX), currentY(currentY) {}

    ~FollowState() override = default;

    void onEnter() {
        std::cout << "FollowState::onEnter" << std::endl;
        currentPath.clear();
        currentPathIndex = 0;
        lastTargetX = -9999;
        lastTargetY = -9999;
        floatPosInitialized = false;
    }

    void onUpdate(float deltaTime) override {
        // Initialiseer float positie bij eerste frame
        if (!floatPosInitialized) {
            floatX = static_cast<float>(parent->getTransform()->getPosition()->getX());
            floatY = static_cast<float>(parent->getTransform()->getPosition()->getY());
            floatPosInitialized = true;
            std::cout << "Initialized float position: (" << floatX << ", " << floatY << ")" << std::endl;
        }

        // Gebruik integer positie voor pathfinding
        int actualX = static_cast<int>(floatX);
        int actualY = static_cast<int>(floatY);

        std::cout << "Float position: (" << floatX << ", " << floatY << ")" << std::endl;
        std::cout << "Int position: (" << actualX << ", " << actualY << ")" << std::endl;
        std::cout << "DeltaTime: " << deltaTime << std::endl;

        // Check of target veranderd is of pad leeg is
        bool needsNewPath = currentPath.empty() ||
                           currentPathIndex >= currentPath.size() ||
                           *targetX != lastTargetX ||
                           *targetY != lastTargetY;

        if (needsNewPath) {
            PathfinderFactory pathfinderFactory;
            auto pathfinder = pathfinderFactory.getPathfinder();

            std::cout << "Calculating path from (" << actualX << ", " << actualY
                      << ") to (" << *targetX << ", " << *targetY << ")" << std::endl;

            int cellSize = 5; // Pas dit aan naar jouw grid size
            currentPath = pathfinder->getPath(scene,
                                             Position{actualX, actualY},
                                             Position{*targetX, *targetY},
                                             cellSize);

            std::cout << "Path size: " << currentPath.size() << std::endl;

            // Debug: print alle waypoints
            for (size_t i = 0; i < currentPath.size(); i++) {
                std::cout << "  Waypoint " << i << ": ("
                         << currentPath[i]->getX() << ", "
                         << currentPath[i]->getY() << ")" << std::endl;
            }

            currentPathIndex = 0;
            lastTargetX = *targetX;
            lastTargetY = *targetY;

            if (currentPath.empty()) {
                std::cout << "No path found!" << std::endl;
                return;
            }
        }

        // Volg het huidige pad
        if (currentPathIndex < currentPath.size()) {
            Position nextPos = *currentPath[currentPathIndex].get();

            // Bereken vector richting volgende punt (gebruik float positie!)
            float dirX = static_cast<float>(nextPos.getX()) - floatX;
            float dirY = static_cast<float>(nextPos.getY()) - floatY;
            float distance = std::sqrt(dirX * dirX + dirY * dirY);

            std::cout << "Moving to waypoint " << currentPathIndex
                      << " at (" << nextPos.getX() << ", " << nextPos.getY()
                      << "), distance: " << distance << std::endl;

            if (distance < 5.0f) {
                // We hebben dit waypoint bereikt, ga naar het volgende
                currentPathIndex++;
                std::cout << "Reached waypoint " << (currentPathIndex - 1)
                         << ", moving to next (index " << currentPathIndex << ")" << std::endl;
            } else if (distance > 0.01f) { // Voorkom deling door nul
                // Normaleer vector
                dirX /= distance;
                dirY /= distance;

                // Bereken beweging in floats (GEEN afrondingsfout!)
                float moveX = dirX * speed * deltaTime;
                float moveY = dirY * speed * deltaTime;

                std::cout << "Direction: (" << dirX << ", " << dirY << ")" << std::endl;
                std::cout << "Movement delta: (" << moveX << ", " << moveY << ")" << std::endl;

                // Voorkom overshoot
                if (std::abs(moveX) > distance) moveX = dirX * distance;
                if (std::abs(moveY) > distance) moveY = dirY * distance;

                // Update float positie
                float oldX = floatX;
                float oldY = floatY;
                floatX += moveX;
                floatY += moveY;

                std::cout << "Float moved from (" << oldX << ", " << oldY
                         << ") to (" << floatX << ", " << floatY << ")" << std::endl;

                // Update GameObject met afgeronde positie
                int newX = static_cast<int>(std::round(floatX));
                int newY = static_cast<int>(std::round(floatY));

                parent->getTransform()->getPosition()->setX(newX);
                parent->getTransform()->getPosition()->setY(newY);

                // Update ook de pointers als ze bestaan
                if (currentX) *currentX = newX;
                if (currentY) *currentY = newY;

                std::cout << "GameObject updated to: (" << newX << ", " << newY << ")" << std::endl;
            }
        } else {
            std::cout << "Reached end of path!" << std::endl;
        }
    }

    const std::vector<std::shared_ptr<ITransition>>& getTransitions() const override {
        static std::vector<std::shared_ptr<ITransition>> empty;
        empty.emplace_back(std::make_shared<SimpleTransition>(currentX, currentY, targetX, targetY));
        return empty;
    }
};

