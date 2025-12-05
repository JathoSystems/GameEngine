#include <iostream>
#include <string>
#include "SaveLoad/SaveLoadSystem.h"
#include "SaveLoad/BaseMemento.h"

struct PlayerData {
    std::string name;
    int health;
    float posX, posY;
};

class GameSaveData : public BaseMemento {
public:
    void setPlayerData(const PlayerData& player) {
        nlohmann::json j;
        j["name"] = player.name;
        j["health"] = player.health;
        j["pos"] = {player.posX, player.posY};
        setData("player", j);
    }

    PlayerData getPlayerData() const {
        PlayerData player;
        auto j = getData("player");
        if (!j.is_null()) {
            player.name = j["name"];
            player.health = j["health"];
            player.posX = j["pos"][0];
            player.posY = j["pos"][1];
        }
        return player;
    }

    void setCurrentLevel(const std::string& level) {
        setData("currentLevel", level);
    }

    std::string getCurrentLevel() const {
        auto j = getData("currentLevel");
        return j.is_string() ? j.get<std::string>() : "Level_01";
    }
};

int main() {
    std::cout << "=== Save/Load System Demo ===\n\n";

    SaveLoadSystem saveSystem;
    saveSystem.initialize("./my_game_saves");

    // --- Create and save custom game data ---
    std::cout << "Creating new save data...\n";
    GameSaveData saveData;
    saveData.setPlayerData({"Hero", 100, 10.5f, 20.3f});
    saveData.setCurrentLevel("Level_03");
    saveData.setData("score", 1500);
    saveData.setData("inventory", nlohmann::json::array({"sword", "shield", "potion"}));

    if (saveSystem.save(saveData, "slot1")) {
        std::cout << "✓ Game saved to slot1!\n\n";
    } else {
        std::cout << "✗ Failed to save game.\n\n";
    }

    // --- Load saved data ---
    std::cout << "Loading save from slot1...\n";
    GameSaveData loadedData;
    if (saveSystem.load(loadedData, "slot1")) {
        std::cout << "✓ Game loaded successfully!\n";

        auto player = loadedData.getPlayerData();
        std::cout << "  Player: " << player.name << "\n";
        std::cout << "  Health: " << player.health << "\n";
        std::cout << "  Position: (" << player.posX << ", " << player.posY << ")\n";
        std::cout << "  Level: " << loadedData.getCurrentLevel() << "\n";
        std::cout << "  Score: " << loadedData.getData("score") << "\n";

        auto inventory = loadedData.getData("inventory");
        std::cout << "  Inventory: ";
        for (const auto& item : inventory) {
            std::cout << item << " ";
        }
        std::cout << "\n\n";
    } else {
        std::cout << "✗ Failed to load game.\n\n";
    }

    // --- List all saves ---
    std::cout << "Available save files:\n";
    auto saves = saveSystem.listSaves();
    for (const auto& save : saves) {
        std::cout << "  - " << save.name
                  << " (saved: " << save.timestamp << ")\n";
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}
