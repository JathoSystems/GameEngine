#ifndef GAMEENGINE_FRAME_H
#define GAMEENGINE_FRAME_H
#include <memory>
#include "GameObjects/Transform/Position.h"
#include "GameObjects/Transform/Size.h"

class Frame {
private:
    std::unique_ptr<Size> _size;
    std::unique_ptr<Position> _position;

    int _srcX;
    int _srcY;

public:
    Frame(int x, int y, int width, int height);

    Size *getSize();

    Position *getPosition() { return _position.get(); }

    int getX() const;

    int getY() const;

    int getWidth() const;

    int getHeight() const;
};

#endif //GAMEENGINE_FRAME_H
