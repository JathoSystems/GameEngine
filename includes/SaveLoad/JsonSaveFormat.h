#ifndef GAMEENGINE_JSONSAVEFORMAT_H
#define GAMEENGINE_JSONSAVEFORMAT_H

#include "ISaveFormat.h"

class JsonSaveFormat : public ISaveFormat {
public:
    std::string serialize(const IMemento &memento) const override;

    bool deserialize(const std::string &data, IMemento &memento) const override;

    std::string getExtension() const override { return ".json"; }
};

#endif //GAMEENGINE_JSONSAVEFORMAT_H
