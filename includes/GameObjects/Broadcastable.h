#ifndef VUURJONGEN_WATERMEISJE_GAME_BROADCASTABLE_H
#define VUURJONGEN_WATERMEISJE_GAME_BROADCASTABLE_H
#include "GameObject.h"

class Broadcastable {
private:
    int _id;
    GameObject *_gameObject;

public:
    Broadcastable(GameObject *gameObject, int id);

    Broadcastable(GameObject *gameObject);

    ~Broadcastable();

    int getId() { return _id; }
};

#endif //VUURJONGEN_WATERMEISJE_GAME_BROADCASTABLE_H
