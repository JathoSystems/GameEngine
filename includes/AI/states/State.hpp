#ifndef GAMEENGINE_STATE_HPP
#define GAMEENGINE_STATE_HPP

#include <functional>
#include <memory>
#include <vector>
#include "../transition/ITransition.h"

class State {
private:
    std::vector<std::unique_ptr<ITransition> > _transitions;

public:
    virtual ~State() = default;

    void onEnter();

    virtual void onUpdate(float deltaTime) = 0;

    void onExit();

    void addTransition(std::unique_ptr<ITransition> transition);
    void addTransition(std::string nextState, std::function<bool()> predicate);

    const std::vector<std::unique_ptr<ITransition> > &getTransitions() const;
};

#endif //GAMEENGINE_STATE_HPP
