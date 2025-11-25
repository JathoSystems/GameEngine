//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_POSITION_H
#define GAMEENGINE_POSITION_H

#include <functional>

class Position {
private:
    int _x;
    int _y;

public:
    Position(int x, int y);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    bool operator==(const Position& other) const {
        return _x == other._x && _y == other._y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

namespace std {
    template<>
    struct hash<Position> {
        size_t operator()(const Position& p) const noexcept {
            return std::hash<int>()(p.getX())
                   ^ (std::hash<int>()(p.getY()) << 1);
        }
    };
}



#endif //GAMEENGINE_POSITION_H