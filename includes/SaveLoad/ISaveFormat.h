#ifndef GAMEENGINE_ISAVEFORMAT_H
#define GAMEENGINE_ISAVEFORMAT_H

#include <string>

class IMemento;

class ISaveFormat {
public:
    virtual ~ISaveFormat() = default;

    virtual std::string serialize(const IMemento& memento) const = 0;
    virtual bool deserialize(const std::string& data, IMemento& memento) const = 0;
    virtual std::string getExtension() const = 0;
};


#endif //GAMEENGINE_ISAVEFORMAT_H