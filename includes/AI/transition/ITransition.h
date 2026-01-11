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
