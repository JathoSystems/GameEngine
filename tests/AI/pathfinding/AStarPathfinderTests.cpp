#include <complex>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "AI/pathfinding/AStarPathfinder.hpp"
#include "GameObjects/GameObject.h"
#include <memory>
#include <vector>

void addObstacle(Scene* scene, int x, int y, int width, int height) {
    auto obstacle = std::make_unique<GameObject>();
    obstacle->getTransform()->getPosition()->setX(x);
    obstacle->getTransform()->getPosition()->setY(y);
    obstacle->getTransform()->getSize()->setWidth(width);
    obstacle->getTransform()->getSize()->setHeight(height);
    scene->addObject(std::move(obstacle));
}

TEST_CASE("AStarPathfinder", "[pathfinding][astar]") {
    AStarPathfinder pathfinder;

    SECTION("Basic Functionality") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Null scene returns empty path") {
            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(nullptr, start, end, 10, "manhattan");
            REQUIRE(path.empty());
        }

        SECTION("Zero cell size returns empty path") {
            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 0, "manhattan");
            REQUIRE(path.empty());
        }

        SECTION("Negative cell size returns empty path") {
            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, -10, "manhattan");
            REQUIRE(path.empty());
        }

        SECTION("Start equals end returns single position path") {
            Position start(50, 50);
            Position end(50, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");
            REQUIRE(path.size() == 1);
            REQUIRE(path[0]->getX() == end.getX());
            REQUIRE(path[0]->getY() == end.getY());
        }
    }

    SECTION("Simple Paths Without Obstacles") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Horizontal path - left to right") {
            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() > 0);

            REQUIRE_THAT(static_cast<double>(path.front()->getY()),
                        Catch::Matchers::WithinAbs(start.getY(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getX()),
                        Catch::Matchers::WithinAbs(end.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getY()),
                        Catch::Matchers::WithinAbs(end.getY(), 10.0));
        }

        SECTION("Vertical path - top to bottom") {
            Position start(50, 0);
            Position end(50, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() > 0);

            REQUIRE_THAT(static_cast<double>(path.front()->getX()),
                        Catch::Matchers::WithinAbs(start.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getX()),
                        Catch::Matchers::WithinAbs(end.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getY()),
                        Catch::Matchers::WithinAbs(end.getY(), 10.0));
        }

        SECTION("Diagonal path - bottom-left to top-right") {
            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE_THAT(static_cast<double>(path.back()->getX()),
                        Catch::Matchers::WithinAbs(end.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getY()),
                        Catch::Matchers::WithinAbs(end.getY(), 10.0));
        }

        SECTION("Short distance path") {
            Position start(10, 10);
            Position end(30, 10);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() >= 2);
        }
    }

    SECTION("Obstacle Avoidance") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Single obstacle in middle - path should go around") {
            addObstacle(scene.get(), 45, 45, 20, 20);

            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());

            bool pathClearOfObstacle = true;
            for (const auto& pos : path) {
                int x = pos->getX();
                int y = pos->getY();
                if (x >= 45 && x <= 65 && y >= 45 && y <= 65) {
                    pathClearOfObstacle = false;
                    break;
                }
            }
            REQUIRE(pathClearOfObstacle);
        }

        SECTION("Wall obstacle - path should navigate around") {
            addObstacle(scene.get(), 50, 0, 10, 80);

            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() > 5);
        }

        SECTION("Multiple obstacles - complex navigation") {
            addObstacle(scene.get(), 30, 30, 15, 15);
            addObstacle(scene.get(), 60, 30, 15, 15);
            addObstacle(scene.get(), 45, 60, 15, 15);

            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Narrow corridor - path should fit through") {
            addObstacle(scene.get(), 0, 40, 40, 10);
            addObstacle(scene.get(), 0, 60, 40, 10);

            Position start(0, 55);
            Position end(50, 55);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
        }
    }

    SECTION("Blocked Paths") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Start position blocked returns empty path") {
            addObstacle(scene.get(), 0, 0, 50, 50);

            Position start(25, 25); // Inside obstacle
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE(path.empty());
        }

        SECTION("End position blocked returns empty path") {
            addObstacle(scene.get(), 90, 90, 20, 20);

            Position start(0, 0);
            Position end(100, 100); // Inside obstacle
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE(path.empty());
        }

        SECTION("U-shaped obstacle enclosing end") {
            addObstacle(scene.get(), 80, 80, 5, 40);  // Left wall
            addObstacle(scene.get(), 80, 80, 40, 5);  // Top wall
            addObstacle(scene.get(), 115, 80, 5, 40); // Right wall

            Position start(0, 0);
            Position end(100, 100); // Inside U-shape
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
        }
    }

    SECTION("Different Cell Sizes") {
        auto scene = std::make_unique<Scene>("main");
        addObstacle(scene.get(), 45, 45, 20, 20);

        SECTION("Small cell size (5) - high precision") {
            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Medium cell size (10) - balanced") {
            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Large cell size (20) - low precision") {
            Position start(0, 50);
            Position end(100, 50);
            auto path = pathfinder.getPath(scene.get(), start, end, 20, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Cell size affects path length") {
            Position start(0, 0);
            Position end(100, 100);

            auto pathSmall = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");
            auto pathLarge = pathfinder.getPath(scene.get(), start, end, 20, "manhattan");

            if (!pathSmall.empty() && !pathLarge.empty()) {
                REQUIRE(pathSmall.size() >= pathLarge.size());
            }
        }
    }

    SECTION("Different Heuristic Types") {
        auto scene = std::make_unique<Scene>("main");
        Position start(0, 0);
        Position end(100, 100);

        SECTION("manhattan heuristic") {
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");
            REQUIRE_FALSE(path.empty());
            REQUIRE_THAT(static_cast<double>(path.back()->getX()),
                        Catch::Matchers::WithinAbs(end.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getY()),
                        Catch::Matchers::WithinAbs(end.getY(), 10.0));
        }

        SECTION("Euclidean heuristic") {
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "euclidean");
            REQUIRE_FALSE(path.empty());
            REQUIRE_THAT(static_cast<double>(path.back()->getX()),
                        Catch::Matchers::WithinAbs(end.getX(), 10.0));
            REQUIRE_THAT(static_cast<double>(path.back()->getY()),
                        Catch::Matchers::WithinAbs(end.getY(), 10.0));
        }
    }

    SECTION("Path Continuity and Correctness") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Path waypoints should be reasonably spaced") {
            Position start(0, 0);
            Position end(100, 0);
            int cellSize = 10;
            auto path = pathfinder.getPath(scene.get(), start, end, cellSize, "manhattan");

            REQUIRE(path.size() >= 2);

            for (size_t i = 1; i < path.size(); i++) {
                int dx = path[i]->getX() - path[i-1]->getX();
                int dy = path[i]->getY() - path[i-1]->getY();
                double distance = std::sqrt(dx*dx + dy*dy);

                REQUIRE(distance <= cellSize * 2);
            }
        }

        SECTION("Path should progress towards goal") {
            Position start(0, 0);
            Position end(100, 100);
            auto path = pathfinder.getPath(scene.get(), start, end, 10, "manhattan");

            REQUIRE(path.size() >= 2);

            int firstDist = std::abs(path.front()->getX() - start.getX()) +
                           std::abs(path.front()->getY() - start.getY());
            int lastDist = std::abs(path.back()->getX() - start.getX()) +
                          std::abs(path.back()->getY() - start.getY());

            REQUIRE(lastDist > firstDist);
        }
    }

    SECTION("Edge Cases") {
        auto scene = std::make_unique<Scene>("main");

        SECTION("Very long distance path") {
            Position start(0, 0);
            Position end(1000, 1000);
            auto path = pathfinder.getPath(scene.get(), start, end, 20, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Path at scene boundaries") {
            Position start(0, 0);
            Position end(5, 5);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
        }

        SECTION("Adjacent start and end positions") {
            Position start(10, 10);
            Position end(15, 10);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
            REQUIRE(path.size() >= 1);
        }

        SECTION("Zigzag obstacle pattern") {
            addObstacle(scene.get(), 20, 0, 10, 40);
            addObstacle(scene.get(), 40, 30, 10, 40);
            addObstacle(scene.get(), 60, 0, 10, 40);

            Position start(0, 35);
            Position end(80, 35);
            auto path = pathfinder.getPath(scene.get(), start, end, 5, "manhattan");

            REQUIRE_FALSE(path.empty());
        }
    }
}