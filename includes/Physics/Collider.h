#ifndef GAMEENGINE_COLLIDER_H
#define GAMEENGINE_COLLIDER_H

enum class ColliderType {
    BOX,
    CIRCLE
};

class Collider {
protected:
    ColliderType _type;
    float _offsetX;
    float _offsetY;

public:
    explicit Collider(ColliderType type) : _type(type), _offsetX(0), _offsetY(0) {
    }

    virtual ~Collider() = default;

    ColliderType getType() const { return _type; }

    void setOffset(float x, float y) {
        _offsetX = x;
        _offsetY = y;
    }

    float getOffsetX() const { return _offsetX; }
    float getOffsetY() const { return _offsetY; }
};

class BoxCollider : public Collider {
private:
    float _width;
    float _height;

public:
    BoxCollider() : Collider(ColliderType::BOX), _width(0), _height(0) {
    }

    BoxCollider(float width, float height)
        : Collider(ColliderType::BOX), _width(width), _height(height) {
    }

    float getWidth() const { return _width; }
    float getHeight() const { return _height; }

    void setSize(float width, float height) {
        _width = width;
        _height = height;
    }
};

class CircleCollider : public Collider {
private:
    float _radius;

public:
    CircleCollider() : Collider(ColliderType::CIRCLE), _radius(0) {
    }

    explicit CircleCollider(float radius)
        : Collider(ColliderType::CIRCLE), _radius(radius) {
    }

    float getRadius() const { return _radius; }
    void setRadius(float radius) { _radius = radius; }
};

#endif //GAMEENGINE_COLLIDER_H
