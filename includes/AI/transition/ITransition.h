//
// Created by kikker234 on 22-11-2025.
//

#ifndef GAMEENGINE_ITRANSITION_H
#define GAMEENGINE_ITRANSITION_H

class State;

class ITransition {
public:
    virtual ~ITransition() = default;

    virtual bool shouldTransition() = 0;

    virtual std::string getTargetState() = 0;
};

#endif //GAMEENGINE_ITRANSITION_H