#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <chrono>

#include "AI/pathfinding/IPathfinder.hpp"
#include "AI/pathfinding/PathfinderFactory.hpp"
#include "Animation/Animation.hpp"
#include "Engine/GameEngine.h"
#include "Events/EventRegistry.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/SpriteRenderer.h"

// ------------------------------
// CONFIG
// ------------------------------
const int MAZE_WIDTH  = 99; // moet oneven zijn
const int MAZE_HEIGHT = 99; // moet oneven zijn
const int CELL_SIZE   = 10;

std::vector<std::vector<int>> maze;

// ------------------------------
// RESOURCE PATH HELPER
// ------------------------------
std::string getResourcePath(const std::string& filename) {
    namespace fs = std::filesystem;
    static std::unordered_map<std::string, std::string> pathCache;

    auto it = pathCache.find(filename);
    if (it != pathCache.end()) return it->second;

    std::vector<std::string> searchPaths = {
        "resources/" + filename,
        "../resources/" + filename,
        "../../resources/" + filename,
        "./GameEngine/resources/" + filename,
        "../GameEngine/resources/" + filename,
        fs::current_path().string() + "/resources/" + filename
    };

    for (const auto& path : searchPaths) {
        if (fs::exists(path)) {
            pathCache[filename] = path;
            return path;
        }
    }

    pathCache[filename] = "resources/" + filename;
    return "resources/" + filename;
}

// ------------------------------
// MAZE GENERATOR
// ------------------------------
void generateMaze() {
    std::cout << "[MAZE] Generating maze...\n";
    maze.assign(MAZE_HEIGHT, std::vector<int>(MAZE_WIDTH, 1));

    std::stack<std::pair<int,int>> stack;
    std::random_device rd;
    std::mt19937 gen(rd());

    int startX = 1, startY = 1;
    maze[startY][startX] = 0;
    stack.push({startX, startY});

    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};

    while (!stack.empty()) {
        auto [x, y] = stack.top();
        std::vector<int> dirs;

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx > 0 && nx < MAZE_WIDTH-1 && ny > 0 && ny < MAZE_HEIGHT-1 && maze[ny][nx] == 1)
                dirs.push_back(i);
        }

        if (dirs.empty()) {
            stack.pop();
            continue;
        }

        std::uniform_int_distribution<int> dist(0, dirs.size() - 1);
        int dir = dirs[dist(gen)];
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        maze[y + dy[dir]/2][x + dx[dir]/2] = 0; // verwijder muur
        maze[ny][nx] = 0;

        stack.push({nx, ny});
    }
}

// ------------------------------
// CLEAR MAZE FROM SCENE
// ------------------------------
void clearMaze(Scene* scene) {
    auto& objects = scene->getObjects();
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj){
                return obj->hasComponent<SpriteRenderer>(); // veronderstel dat walls/path SpriteRenderer hebben
            }),
        objects.end()
    );
}

// ------------------------------
// CREATE MAZE IN SCENE
// ------------------------------
void createMaze(Scene* scene, const std::string& wallTexture) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze[y][x] == 1) {
                auto wall = std::make_unique<GameObject>();
                auto renderer = std::make_unique<SpriteRenderer>(wallTexture);

                wall->getTransform()->getPosition()->setX(x * CELL_SIZE);
                wall->getTransform()->getPosition()->setY(y * CELL_SIZE);
                wall->getTransform()->getSize()->setWidth(CELL_SIZE);
                wall->getTransform()->getSize()->setHeight(CELL_SIZE);

                wall->addComponent(std::move(renderer));
                scene->addObject(std::move(wall));
            }
        }
    }
}

// ------------------------------
// VISUALIZE PATH
// ------------------------------
void visualizePath(Scene* scene,
                   const std::vector<std::unique_ptr<Position>>& path,
                   const std::string& pathTexture) {
    for (const auto& pos : path) {
        auto marker = std::make_unique<GameObject>();
        auto renderer = std::make_unique<SpriteRenderer>(pathTexture);

        marker->getTransform()->getPosition()->setX(pos->getX() * CELL_SIZE);
        marker->getTransform()->getPosition()->setY(pos->getY() * CELL_SIZE);
        marker->getTransform()->getSize()->setWidth(CELL_SIZE);
        marker->getTransform()->getSize()->setHeight(CELL_SIZE);

        marker->addComponent(std::move(renderer));
        scene->addObject(std::move(marker));
    }
}

// ------------------------------
// MAIN
// ------------------------------
int main() {
    try {
        auto engine = std::make_unique<GameEngine>();
        engine->init("A* Pathfinding Maze Demo", 1800, 1000);

        auto scene = std::make_unique<Scene>("main");

        std::string wallTexture = getResourcePath("square_blue.png");
        std::string pathTexture = getResourcePath("square.png");

        generateMaze();
        Position start(1,1);
        Position end(MAZE_WIDTH-2, MAZE_HEIGHT-2);
        maze[start.getY()][start.getX()] = 0;
        maze[end.getY()][end.getX()] = 0;

        createMaze(scene.get(), wallTexture);

        auto pathfinder = PathfinderFactory::getPathfinder();
        auto path = pathfinder->getPath(scene.get(), start, end, CELL_SIZE);
        if (!path.empty()) visualizePath(scene.get(), path, pathTexture);

        // Voeg keypress callback toe
        engine->setKeyPressedCallback([&](int key){
            if (key == 'Q' || key == 'q') {
                std::cout << "[INPUT] Q pressed - regenerating maze\n";
                clearMaze(scene.get());
                generateMaze();
                maze[start.getY()][start.getX()] = 0;
                maze[end.getY()][end.getX()] = 0;
                createMaze(scene.get(), wallTexture);

                auto newPath = pathfinder->getPath(scene.get(), start, end, CELL_SIZE);
                if (!newPath.empty()) visualizePath(scene.get(), newPath, pathTexture);
            }
        });

        engine->addScene(std::move(scene));
        engine->start();
    }
    catch (const std::exception &e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
    }

    return 0;
}
