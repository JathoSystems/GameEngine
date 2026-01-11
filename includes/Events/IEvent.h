#ifndef EVENTS_IEVENT_H
#define EVENTS_IEVENT_H
#include <string>
#include <vector>
#include <memory>
#include "GameObjects/GameObject.h"

using Package = std::vector<uint8_t>;
using Data = std::vector<uint8_t>;

class IEvent {
protected:
    std::string name;

public:
    virtual ~IEvent() = default;

    virtual std::string getName() const = 0;

    virtual Package serialize() const = 0;

    virtual Data deserialize(const Package &package) = 0;

    virtual void apply(GameObject *gameObject) = 0;
};
#endif
