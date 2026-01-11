#ifndef GAMEENGINE_COLLISIONDATA_H
#define GAMEENGINE_COLLISIONDATA_H

class GameObject;

struct CollisionData {
    GameObject *other;
    float normalX;
    float normalY;
    bool isTouching;

    CollisionData(GameObject *obj, float nx, float ny, bool touching)
        : other(obj), normalX(nx), normalY(ny), isTouching(touching) {
    }
};

#endif
