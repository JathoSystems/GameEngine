#ifndef GAMEENGINE_IMEMENTO_H
#define GAMEENGINE_IMEMENTO_H

#include <string>
#include <nlohmann/json.hpp>

class IMemento {
public:
    virtual ~IMemento() = default;

    virtual std::string getTimestamp() const = 0;

    virtual void setTimestamp(const std::string &timestamp) = 0;

    virtual nlohmann::json toJson() const = 0;

    virtual void fromJson(const nlohmann::json &j) = 0;

    virtual bool isValid() const = 0;
};

#endif //GAMEENGINE_IMEMENTO_H
