#ifndef GAMEENGINE_SAVECARETAKER_H
#define GAMEENGINE_SAVECARETAKER_H

#include "IMemento.h"
#include "ISaveFormat.h"
#include <string>
#include <memory>
#include <vector>

struct SaveFileInfo {
    std::string name;
    std::string path;
    std::string timestamp;
};

class SaveCaretaker {
public:
    SaveCaretaker(std::string saveDirectory, std::unique_ptr<ISaveFormat> format);

    bool save(const IMemento &memento, const std::string &slotName);

    bool load(IMemento &memento, const std::string &slotName) const;

    std::vector<SaveFileInfo> listSaves() const;

    bool deleteSave(const std::string &slotName);

    bool exists(const std::string &slotName) const;

    void setAutoSaveInterval(float seconds) { m_autoSaveInterval = seconds; }
    float getAutoSaveInterval() const { return m_autoSaveInterval; }

    std::string getSaveDirectory() const { return m_saveDirectory; }

private:
    std::string m_saveDirectory;
    std::unique_ptr<ISaveFormat> m_formatStrategy;
    float m_autoSaveInterval = 60.0f;

    std::string getSavePath(const std::string &slotName) const;

    bool writeAtomically(const std::string &path, const std::string &data) const;

    std::string iso8601UtcNow() const;
};

#endif //GAMEENGINE_SAVECARETAKER_H
