#include "SaveLoad/SaveCaretaker.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

SaveCaretaker::SaveCaretaker(std::string saveDirectory, std::unique_ptr<ISaveFormat> format)
    : m_saveDirectory(std::move(saveDirectory)), m_formatStrategy(std::move(format)) {
    fs::create_directories(m_saveDirectory);
}

std::string SaveCaretaker::getSavePath(const std::string& slotName) const {
    return (fs::path(m_saveDirectory) / (slotName + m_formatStrategy->getExtension())).string();
}

std::string SaveCaretaker::iso8601UtcNow() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%FT%TZ");
    return ss.str();
}

bool SaveCaretaker::writeAtomically(const std::string& path, const std::string& data) const {
    try {
        fs::path target(path);
        fs::create_directories(target.parent_path());

        fs::path tmp = target;
        tmp += ".tmp";

        {
            std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
            if (!out) return false;
            out.write(data.data(), static_cast<std::streamsize>(data.size()));
            if (!out.good()) return false;
        }

        fs::rename(tmp, target);
        return true;
    } catch (...) {
        return false;
    }
}

bool SaveCaretaker::save(const IMemento& memento, const std::string& slotName) {
    try {
        // Auto-set timestamp if empty
        if (memento.getTimestamp().empty()) {
            const_cast<IMemento&>(memento).setTimestamp(iso8601UtcNow());
        }

        std::string serialized = m_formatStrategy->serialize(memento);
        return writeAtomically(getSavePath(slotName), serialized);
    } catch (...) {
        return false;
    }
}

bool SaveCaretaker::load(IMemento& memento, const std::string& slotName) const {
    try {
        fs::path p(getSavePath(slotName));
        if (!fs::exists(p) || !fs::is_regular_file(p)) return false;

        std::ifstream in(p, std::ios::binary);
        if (!in) return false;

        std::string buffer((std::istreambuf_iterator<char>(in)), {});
        return m_formatStrategy->deserialize(buffer, memento);
    } catch (...) {
        return false;
    }
}

std::vector<SaveFileInfo> SaveCaretaker::listSaves() const {
    std::vector<SaveFileInfo> result;
    try {
        if (!fs::exists(m_saveDirectory)) return result;

        for (const auto& entry : fs::directory_iterator(m_saveDirectory)) {
            if (entry.is_regular_file() &&
                entry.path().extension() == m_formatStrategy->getExtension()) {

                SaveFileInfo info;
                info.name = entry.path().stem().string();
                info.path = entry.path().string();

                // Try to read timestamp without full deserialization
                try {
                    std::ifstream in(entry.path(), std::ios::binary);
                    std::string content((std::istreambuf_iterator<char>(in)), {});
                    auto j = nlohmann::json::parse(content);
                    if (j.contains("timestamp")) {
                        info.timestamp = j["timestamp"];
                    }
                } catch (...) {}

                result.push_back(info);
            }
        }
    } catch (...) {}
    return result;
}

bool SaveCaretaker::deleteSave(const std::string& slotName) {
    try {
        return fs::remove(getSavePath(slotName));
    } catch (...) {
        return false;
    }
}

bool SaveCaretaker::exists(const std::string& slotName) const {
    return fs::exists(getSavePath(slotName));
}
