#include <catch2/catch_test_macros.hpp>
#include "SaveLoad/SaveLoadSystem.h"
#include "SaveLoad/BaseMemento.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class TestMemento : public BaseMemento {
public:
    void setPlayerName(const std::string& name) {
        setData("playerName", name);
    }

    std::string getPlayerName() const {
        auto j = getData("playerName");
        return j.is_string() ? j.get<std::string>() : "";
    }

    void setScore(int score) {
        setData("score", score);
    }

    int getScore() const {
        auto j = getData("score");
        return j.is_number_integer() ? j.get<int>() : 0;
    }

    void setPosition(float x, float y) {
        setData("position", nlohmann::json::array({x, y}));
    }

    std::pair<float, float> getPosition() const {
        auto j = getData("position");
        if (j.is_array() && j.size() == 2) {
            return {j[0].get<float>(), j[1].get<float>()};
        }
        return {0.0f, 0.0f};
    }
};

static void cleanupTestDirectory(const std::string& dir) {
    if (fs::exists(dir)) {
        fs::remove_all(dir);
    }
}

TEST_CASE("SaveLoadSystem", "[saveload][system]") {
    const std::string testDir = "./test_saves";
    cleanupTestDirectory(testDir);

    SaveLoadSystem saveSystem;

    SECTION("Initialization") {
        SECTION("Initialize creates save directory") {
            REQUIRE_NOTHROW(saveSystem.initialize(testDir));
            REQUIRE(fs::exists(testDir));
            REQUIRE(fs::is_directory(testDir));
        }

        SECTION("Initialize with nested directory") {
            std::string nestedDir = testDir + "/nested/deep";
            saveSystem.initialize(nestedDir);
            REQUIRE(fs::exists(nestedDir));
        }
    }

    SECTION("Save Operations") {
        saveSystem.initialize(testDir);

        SECTION("Save simple data") {
            TestMemento memento;
            memento.setPlayerName("TestPlayer");
            memento.setScore(100);

            REQUIRE(saveSystem.save(memento, "slot1"));
            REQUIRE(saveSystem.exists("slot1"));
        }

        SECTION("Save complex nested data") {
            TestMemento memento;
            memento.setPlayerName("Hero");
            memento.setScore(9999);
            memento.setPosition(10.5f, 20.3f);
            memento.setData("inventory", nlohmann::json::array({"sword", "shield", "potion"}));
            memento.setData("quests", nlohmann::json::object({
                {"quest1", "completed"},
                {"quest2", "active"}
            }));

            REQUIRE(saveSystem.save(memento, "slot2"));
            REQUIRE(saveSystem.exists("slot2"));
        }

        SECTION("Save overwrites existing file") {
            TestMemento memento1;
            memento1.setPlayerName("FirstSave");
            memento1.setScore(100);

            REQUIRE(saveSystem.save(memento1, "overwrite_test"));

            TestMemento memento2;
            memento2.setPlayerName("SecondSave");
            memento2.setScore(200);

            REQUIRE(saveSystem.save(memento2, "overwrite_test"));

            TestMemento loaded;
            REQUIRE(saveSystem.load(loaded, "overwrite_test"));
            REQUIRE(loaded.getPlayerName() == "SecondSave");
            REQUIRE(loaded.getScore() == 200);
        }

        SECTION("Save auto-sets timestamp") {
            TestMemento memento;
            memento.setPlayerName("TimestampTest");
            REQUIRE(memento.getTimestamp().empty());

            REQUIRE(saveSystem.save(memento, "timestamp_test"));
            REQUIRE_FALSE(memento.getTimestamp().empty());
        }
    }

    SECTION("Load Operations") {
        saveSystem.initialize(testDir);

        SECTION("Load simple data") {
            TestMemento saveMemento;
            saveMemento.setPlayerName("LoadTest");
            saveMemento.setScore(500);
            saveSystem.save(saveMemento, "load_test");

            TestMemento loadMemento;
            REQUIRE(saveSystem.load(loadMemento, "load_test"));
            REQUIRE(loadMemento.getPlayerName() == "LoadTest");
            REQUIRE(loadMemento.getScore() == 500);
        }

        SECTION("Load complex nested data") {
            TestMemento saveMemento;
            saveMemento.setPlayerName("ComplexTest");
            saveMemento.setPosition(15.7f, 22.1f);
            saveMemento.setData("level", "Level_05");
            saveMemento.setData("settings", nlohmann::json::object({
                {"volume", 80},
                {"quality", "high"}
            }));
            saveSystem.save(saveMemento, "complex_test");

            TestMemento loadMemento;
            REQUIRE(saveSystem.load(loadMemento, "complex_test"));
            REQUIRE(loadMemento.getPlayerName() == "ComplexTest");

            auto [x, y] = loadMemento.getPosition();
            REQUIRE(x == 15.7f);
            REQUIRE(y == 22.1f);

            auto level = loadMemento.getData("level");
            REQUIRE(level == "Level_05");

            auto settings = loadMemento.getData("settings");
            REQUIRE(settings["volume"] == 80);
            REQUIRE(settings["quality"] == "high");
        }

        SECTION("Load non-existent file returns false") {
            TestMemento memento;
            REQUIRE_FALSE(saveSystem.load(memento, "does_not_exist"));
        }

        SECTION("Load preserves version") {
            TestMemento saveMemento;
            saveMemento.setVersion(2);
            saveMemento.setPlayerName("VersionTest");
            saveSystem.save(saveMemento, "version_test");

            TestMemento loadMemento;
            REQUIRE(saveSystem.load(loadMemento, "version_test"));
            REQUIRE(loadMemento.getVersion() == 2);
        }
    }

    SECTION("Multiple Slots") {
        saveSystem.initialize(testDir);

        SECTION("Save and load multiple slots") {
            TestMemento slot1;
            slot1.setPlayerName("Player1");
            slot1.setScore(100);

            TestMemento slot2;
            slot2.setPlayerName("Player2");
            slot2.setScore(200);

            TestMemento slot3;
            slot3.setPlayerName("Player3");
            slot3.setScore(300);

            REQUIRE(saveSystem.save(slot1, "slot1"));
            REQUIRE(saveSystem.save(slot2, "slot2"));
            REQUIRE(saveSystem.save(slot3, "slot3"));

            TestMemento loadedSlot2;
            REQUIRE(saveSystem.load(loadedSlot2, "slot2"));
            REQUIRE(loadedSlot2.getPlayerName() == "Player2");
            REQUIRE(loadedSlot2.getScore() == 200);
        }
    }

    SECTION("List Saves") {
        saveSystem.initialize(testDir);

        SECTION("List empty directory returns empty vector") {
            auto saves = saveSystem.listSaves();
            REQUIRE(saves.empty());
        }

        SECTION("List saves returns all save files") {
            TestMemento m1, m2, m3;
            m1.setPlayerName("Save1");
            m2.setPlayerName("Save2");
            m3.setPlayerName("Save3");

            saveSystem.save(m1, "save1");
            saveSystem.save(m2, "save2");
            saveSystem.save(m3, "save3");

            auto saves = saveSystem.listSaves();
            REQUIRE(saves.size() == 3);

            bool foundSave1 = false, foundSave2 = false, foundSave3 = false;
            for (const auto& save : saves) {
                if (save.name == "save1") foundSave1 = true;
                if (save.name == "save2") foundSave2 = true;
                if (save.name == "save3") foundSave3 = true;
                REQUIRE_FALSE(save.path.empty());
            }
            REQUIRE(foundSave1);
            REQUIRE(foundSave2);
            REQUIRE(foundSave3);
        }

        SECTION("List saves includes timestamp") {
            TestMemento memento;
            memento.setPlayerName("TimestampTest");
            saveSystem.save(memento, "timestamp_list");

            auto saves = saveSystem.listSaves();
            REQUIRE(saves.size() == 1);
            REQUIRE_FALSE(saves[0].timestamp.empty());
        }
    }

    SECTION("Delete Operations") {
        saveSystem.initialize(testDir);

        SECTION("Delete existing save") {
            TestMemento memento;
            memento.setPlayerName("ToDelete");
            saveSystem.save(memento, "delete_test");

            REQUIRE(saveSystem.exists("delete_test"));
            REQUIRE(saveSystem.deleteSave("delete_test"));
            REQUIRE_FALSE(saveSystem.exists("delete_test"));
        }

        SECTION("Delete non-existent save returns false") {
            REQUIRE_FALSE(saveSystem.deleteSave("does_not_exist"));
        }

        SECTION("Delete does not affect other saves") {
            TestMemento m1, m2;
            m1.setPlayerName("Keep");
            m2.setPlayerName("Delete");

            saveSystem.save(m1, "keep");
            saveSystem.save(m2, "delete");

            REQUIRE(saveSystem.deleteSave("delete"));
            REQUIRE(saveSystem.exists("keep"));
            REQUIRE_FALSE(saveSystem.exists("delete"));
        }
    }

    SECTION("Exists Operations") {
        saveSystem.initialize(testDir);

        SECTION("Exists returns true for existing save") {
            TestMemento memento;
            memento.setPlayerName("ExistsTest");
            saveSystem.save(memento, "exists_test");

            REQUIRE(saveSystem.exists("exists_test"));
        }

        SECTION("Exists returns false for non-existent save") {
            REQUIRE_FALSE(saveSystem.exists("does_not_exist"));
        }
    }

    SECTION("Edge Cases") {
        saveSystem.initialize(testDir);

        SECTION("Save with special characters in slot name") {
            TestMemento memento;
            memento.setPlayerName("SpecialChars");
            REQUIRE(saveSystem.save(memento, "slot_with-dashes_123"));
        }

        SECTION("Save empty memento") {
            TestMemento emptyMemento;
            REQUIRE(saveSystem.save(emptyMemento, "empty"));

            TestMemento loaded;
            REQUIRE(saveSystem.load(loaded, "empty"));
        }

        SECTION("Save very large data") {
            TestMemento memento;
            nlohmann::json largeArray = nlohmann::json::array();
            for (int i = 0; i < 1000; ++i) {
                largeArray.push_back(i);
            }
            memento.setData("largeArray", largeArray);

            REQUIRE(saveSystem.save(memento, "large_data"));

            TestMemento loaded;
            REQUIRE(saveSystem.load(loaded, "large_data"));
            auto loadedArray = loaded.getData("largeArray");
            REQUIRE(loadedArray.size() == 1000);
        }
    }

    cleanupTestDirectory(testDir);
}
