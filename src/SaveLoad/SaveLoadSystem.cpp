#include "SaveLoad/SaveLoadSystem.h"
#include "SaveLoad/JsonSaveFormat.h"

void SaveLoadSystem::initialize(const std::string &saveDirectory) {
    auto format = std::make_unique<JsonSaveFormat>();
    m_caretaker = std::make_unique<SaveCaretaker>(saveDirectory, std::move(format));
}

bool SaveLoadSystem::save(IMemento &memento, const std::string &slotName) {
    return m_caretaker->save(memento, slotName);
}

bool SaveLoadSystem::load(IMemento &memento, const std::string &slotName) {
    return m_caretaker->load(memento, slotName);
}

std::vector<SaveFileInfo> SaveLoadSystem::listSaves() const {
    return m_caretaker->listSaves();
}

bool SaveLoadSystem::deleteSave(const std::string &slotName) {
    return m_caretaker->deleteSave(slotName);
}

bool SaveLoadSystem::exists(const std::string &slotName) const {
    return m_caretaker->exists(slotName);
}
