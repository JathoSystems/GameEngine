//
// Created by jusra on 12-12-2025.
//

#ifndef VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP
#define VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP
#include <map>

#include "GameObjects/GameObject.h"

class ObjectRegistry {
public:
    static ObjectRegistry& getInstance();

    GameObject* getObject(int key);
    int registerObject(GameObject* obj);
    void removeObject(int key);

private:
    ObjectRegistry() = default;

    int generateToken();

    std::unordered_map<int, GameObject*> _objects;
};


#endif //VUURJONGEN_WATERMEISJE_GAME_OBJECTREGISTRY_HPP