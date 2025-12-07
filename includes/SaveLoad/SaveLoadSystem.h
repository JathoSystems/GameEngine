#ifndef GAMEENGINE_SAVELOADSYSTEM_H
#define GAMEENGINE_SAVELOADSYSTEM_H

#include "SaveCaretaker.h"
#include "IMemento.h"
#include <memory>
#include <functional>

class SaveLoadSystem {
public:
    SaveLoadSystem() = default;

    void initialize(const std::string& saveDirectory);

    // Generic save/load
    bool save(IMemento& memento, const std::string& slotName);
    bool load(IMemento& memento, const std::string& slotName);

    std::vector<SaveFileInfo> listSaves() const;
    bool deleteSave(const std::string& slotName);
    bool exists(const std::string& slotName) const;

    SaveCaretaker& getCaretaker() { return *m_caretaker; }

private:
    std::unique_ptr<SaveCaretaker> m_caretaker;
};


#endif //GAMEENGINE_SAVELOADSYSTEM_H