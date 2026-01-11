#ifndef GAMEENGINE_BASEMEMENTO_H
#define GAMEENGINE_BASEMEMENTO_H
#include "IMemento.h"
#include <nlohmann/json.hpp>
#include <chrono>

class BaseMemento : public IMemento {
public:
    BaseMemento() : m_version(1) {
    }

    virtual ~BaseMemento() = default;

    std::string getTimestamp() const override { return m_timestamp; }
    void setTimestamp(const std::string &timestamp) override { m_timestamp = timestamp; }

    int getVersion() const { return m_version; }
    void setVersion(int version) { m_version = version; }

    void setData(const std::string &key, const nlohmann::json &value) {
        m_data[key] = value;
    }

    nlohmann::json getData(const std::string &key) const {
        return m_data.contains(key) ? m_data[key] : nlohmann::json();
    }

    bool hasData(const std::string &key) const {
        return m_data.contains(key);
    }

    nlohmann::json toJson() const override {
        nlohmann::json j;
        j["version"] = m_version;
        j["timestamp"] = m_timestamp;
        j["data"] = m_data;
        return j;
    }

    void fromJson(const nlohmann::json &j) override {
        if (j.contains("version")) m_version = j["version"];
        if (j.contains("timestamp")) m_timestamp = j["timestamp"];
        if (j.contains("data")) m_data = j["data"];
    }

    bool isValid() const override {
        return m_version > 0 && !m_timestamp.empty();
    }

protected:
    std::string m_timestamp;
    int m_version;
    nlohmann::json m_data = nlohmann::json::object();
};

#endif //GAMEENGINE_BASEMEMENTO_H
