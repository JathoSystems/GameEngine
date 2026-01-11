#ifndef VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP
#define VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP

#include "GameObjects/GameObject.h"

class ObjectRegistry {
public:
    static ObjectRegistry &getInstance();

    GameObject *getObject(int key);

    void insert(GameObject *obj, int key);

    int registerObject(GameObject *obj);

    void removeObject(int key);

    void clear();

private:
    int MAX_BROADCASTABLE = 1000;
    int _nextTokenId = 1;

    ObjectRegistry() = default;

    int generateToken();

    std::unordered_map<int, GameObject *> _objects;
};


#endif //VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP
