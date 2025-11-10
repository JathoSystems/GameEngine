//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_POSITION_H
#define GAMEENGINE_POSITION_H

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
};

#endif //GAMEENGINE_POSITION_H