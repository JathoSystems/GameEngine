#pragma once
#include <string>
#include <vector>
#include <cstdint>

// Forward declarations
class GameObject;

// Type alias for serialized data
using Package = std::vector<uint8_t>;
using Data = std::vector<uint8_t>;

class IEvent {
protected:
    std::string name;

public:
    virtual ~IEvent() = default;
    
    virtual std::string getName() const = 0;
    virtual Package serialize() const = 0;
    virtual Data deserialize(const Package& package) const = 0;
    virtual void apply(GameObject* gameObject) = 0;
};
