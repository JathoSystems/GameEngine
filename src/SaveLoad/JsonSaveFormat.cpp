#include "SaveLoad/JsonSaveFormat.h"
#include "SaveLoad/IMemento.h"
#include <nlohmann/json.hpp>

std::string JsonSaveFormat::serialize(const IMemento& memento) const {
    return memento.toJson().dump(2);
}

bool JsonSaveFormat::deserialize(const std::string& data, IMemento& memento) const {
    try {
        nlohmann::json j = nlohmann::json::parse(data);
        memento.fromJson(j);
        return memento.isValid();
    } catch (...) {
        return false;
    }
}
