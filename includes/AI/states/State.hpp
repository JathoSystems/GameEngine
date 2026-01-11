#ifndef GAMEENGINE_STATE_HPP
#define GAMEENGINE_STATE_HPP

#include <functional>
#include <memory>
#include <vector>
#include "../transition/ITransition.h"
#include "GameObjects/GameObject.h"

class AiController;

class State {
private:
    std::vector<std::unique_ptr<ITransition> > _transitions;

protected:
    GameObject *_object;

public:
    virtual ~State() = default;

    virtual void onEnter();

    virtual void onUpdate(float deltaTime) = 0;

    virtual void onExit();

    void addTransition(std::unique_ptr<ITransition> transition);

    void addTransition(const std::string &nextState, const std::function<bool()> predicate);

    const std::vector<std::unique_ptr<ITransition> > &getTransitions() const;

    void setGameObject(GameObject *object);
};

#endif //GAMEENGINE_STATE_HPP
