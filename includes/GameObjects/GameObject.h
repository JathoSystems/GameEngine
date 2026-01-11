#ifndef GAMEENGINE_GAMEOBJECT_H
#define GAMEENGINE_GAMEOBJECT_H

#include <iostream>
#include <memory>
#include <vector>
#include "Collision/CollisionData.h"
#include "Component/Component.h"
#include "Transform/Transform.h"

class GameObject {
private:
    int _layer = 1;
    bool _destroy = false;
    std::unique_ptr<GameObject> _parent;
    std::vector<std::unique_ptr<Component> > _components;
    std::unique_ptr<Transform> _transform = std::make_unique<Transform>();

public:
    virtual ~GameObject() {
        std::cout << "Destroying GameObject at " << this << std::endl;
    }

    void addComponent(std::unique_ptr<Component> component);

    void render(const std::unique_ptr<Window> &window);

    virtual void update(float delta);

    void setLayer(int layer);

    int getLayer();

    Transform *getTransform();

    virtual void onCollisionEnter(const CollisionData &collision) {
    }

    virtual void onCollisionExit(const CollisionData &collision) {
    }

    bool shouldBeDestroyed() const { return _destroy; }

    void destroy() {
        if (_destroy) {
            std::cout << "Object " << this << " marked for destruction\n";
        }

        _destroy = true;
    }

    template<typename T>
    std::vector<T *> getComponents() {
        std::vector<T *> foundComponents;
        for (const std::unique_ptr<Component> &component: _components) {
            if (T *cast = dynamic_cast<T *>(component.get())) {
                foundComponents.push_back(cast);
            }
        }

        return foundComponents;
    }

    template<typename T>
    T *getComponent() {
        for (const std::unique_ptr<Component> &component: _components) {
            if (T *cast = dynamic_cast<T *>(component.get())) {
                return cast;
            }
        }

        return nullptr;
    }

    template<typename T>
    void removeComponent(bool removeAll) {
        T *firstRemoved = nullptr;

        for (auto it = _components.begin(); it != _components.end();) {
            if (T *cast = dynamic_cast<T *>(it->get())) {
                T *removed = cast;
                it = _components.erase(it);

                if (firstRemoved == nullptr)
                    firstRemoved = removed;

                if (!removeAll)
                    break;
            } else {
                ++it;
            }
        }
    }
};

#endif //GAMEENGINE_GAMEOBJECT_H
