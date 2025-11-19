//
// Created by kikker234 on 08-11-2025.
//

#ifndef GAMEENGINE_BEHAVIOUR_H
#define GAMEENGINE_BEHAVIOUR_H
#include <memory>

#include "Component.h"

class Behaviour : public Component {
public:
    void update(float deltatime) override;
    void render(const std::unique_ptr<Window> &window, const Viewport* viewport) override;

    virtual void onUpdate() = 0;
};

#endif //GAMEENGINE_BEHAVIOUR_H