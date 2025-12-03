#ifndef GAMEENGINE_MOCKEVENT_H
#define GAMEENGINE_MOCKEVENT_H

#include <memory>
#include <string>
#include "Events/IEvent.h"
#include "GameObjects/GameObject.h"

class MockEvent : public IEvent {
public:
    mutable bool serializeCalled = false;
    mutable bool deserializeCalled = false;
    mutable bool applyCalled = false;
    std::string eventName;

    MockEvent(const std::string& name) : eventName(name) {}

    std::string getName() const override {
        return eventName;
    }

    Package serialize() const override {
        serializeCalled = true;
        return Package{0x01};
    }

    Data deserialize(const Package& package) const override {
        (void)package;
        deserializeCalled = true;
        return Data{0x02};
    }

    void apply(std::shared_ptr<GameObject> gameObject) override {
        (void)gameObject;
        applyCalled = true;
    }
};
#endif //GAMEENGINE_MOCKEVENT_H
