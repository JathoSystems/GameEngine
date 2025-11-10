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
    Frame(int x, int y, int width, int height)
        : _srcX(x), _srcY(y), _size(std::make_unique<Size>(width, height)),
          _position(std::make_unique<Position>(0,0)) {}

    Size* getSize() { return _size.get(); }
    Position* getPosition() { return _position.get(); }

    int getX() const { return _srcX; }
    int getY() const { return _srcY; }
    int getWidth() const { return _size->getWidth(); }
    int getHeight() const { return _size->getHeight(); }
};

#endif //GAMEENGINE_FRAME_H
