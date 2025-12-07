#include <catch2/catch_test_macros.hpp>
#include "SaveLoad/BaseMemento.h"
#include <nlohmann/json.hpp>

TEST_CASE("BaseMemento", "[saveload][memento]") {
    SECTION("Construction") {
        SECTION("Default constructor sets version to 1") {
            BaseMemento memento;
            REQUIRE(memento.getVersion() == 1);
        }

        SECTION("Default timestamp is empty") {
            BaseMemento memento;
            REQUIRE(memento.getTimestamp().empty());
        }
    }

    SECTION("Timestamp Operations") {
        BaseMemento memento;

        SECTION("Set and get timestamp") {
            memento.setTimestamp("2025-01-01T12:00:00Z");
            REQUIRE(memento.getTimestamp() == "2025-01-01T12:00:00Z");
        }

        SECTION("Update timestamp") {
            memento.setTimestamp("2025-01-01T12:00:00Z");
            memento.setTimestamp("2025-01-02T14:30:00Z");
            REQUIRE(memento.getTimestamp() == "2025-01-02T14:30:00Z");
        }
    }

    SECTION("Version Operations") {
        BaseMemento memento;

        SECTION("Set and get version") {
            memento.setVersion(5);
            REQUIRE(memento.getVersion() == 5);
        }

        SECTION("Update version") {
            memento.setVersion(1);
            memento.setVersion(10);
            REQUIRE(memento.getVersion() == 10);
        }
    }

    SECTION("Data Operations") {
        BaseMemento memento;

        SECTION("Set and get string data") {
            memento.setData("name", "TestPlayer");
            REQUIRE(memento.getData("name") == "TestPlayer");
        }

        SECTION("Set and get integer data") {
            memento.setData("score", 1500);
            REQUIRE(memento.getData("score") == 1500);
        }

        SECTION("Set and get float data") {
            memento.setData("health", 95.5f);
            REQUIRE(memento.getData("health") == 95.5f);
        }

        SECTION("Set and get boolean data") {
            memento.setData("isAlive", true);
            REQUIRE(memento.getData("isAlive") == true);
        }

        SECTION("Set and get array data") {
            nlohmann::json arr = nlohmann::json::array({"sword", "shield", "potion"});
            memento.setData("inventory", arr);

            auto retrieved = memento.getData("inventory");
            REQUIRE(retrieved.is_array());
            REQUIRE(retrieved.size() == 3);
            REQUIRE(retrieved[0] == "sword");
        }

        SECTION("Set and get object data") {
            nlohmann::json obj = nlohmann::json::object({
                {"x", 10.5},
                {"y", 20.3}
            });
            memento.setData("position", obj);

            auto retrieved = memento.getData("position");
            REQUIRE(retrieved.is_object());
            REQUIRE(retrieved["x"] == 10.5);
            REQUIRE(retrieved["y"] == 20.3);
        }

        SECTION("Get non-existent key returns null") {
            auto result = memento.getData("nonexistent");
            REQUIRE(result.is_null());
        }

        SECTION("Update existing data") {
            memento.setData("score", 100);
            memento.setData("score", 200);
            REQUIRE(memento.getData("score") == 200);
        }
    }

    SECTION("hasData Operations") {
        BaseMemento memento;

        SECTION("hasData returns true for existing key") {
            memento.setData("testKey", "value");
            REQUIRE(memento.hasData("testKey"));
        }

        SECTION("hasData returns false for non-existent key") {
            REQUIRE_FALSE(memento.hasData("nonexistent"));
        }

        SECTION("hasData after deletion") {
            memento.setData("temp", 123);
            REQUIRE(memento.hasData("temp"));
        }
    }

    SECTION("JSON Serialization") {
        BaseMemento memento;

        SECTION("toJson includes version, timestamp, and data") {
            memento.setVersion(2);
            memento.setTimestamp("2025-01-01T12:00:00Z");
            memento.setData("name", "Player");
            memento.setData("score", 500);

            auto json = memento.toJson();
            REQUIRE(json["version"] == 2);
            REQUIRE(json["timestamp"] == "2025-01-01T12:00:00Z");
            REQUIRE(json["data"]["name"] == "Player");
            REQUIRE(json["data"]["score"] == 500);
        }

        SECTION("toJson with nested data") {
            memento.setData("player", nlohmann::json::object({
                {"name", "Hero"},
                {"stats", nlohmann::json::object({
                    {"hp", 100},
                    {"mp", 50}
                })}
            }));

            auto json = memento.toJson();
            REQUIRE(json["data"]["player"]["name"] == "Hero");
            REQUIRE(json["data"]["player"]["stats"]["hp"] == 100);
        }
    }

    SECTION("JSON Deserialization") {
        SECTION("fromJson restores all fields") {
            nlohmann::json json = {
                {"version", 3},
                {"timestamp", "2025-01-15T10:30:00Z"},
                {"data", {
                    {"name", "LoadedPlayer"},
                    {"level", 10}
                }}
            };

            BaseMemento memento;
            memento.fromJson(json);

            REQUIRE(memento.getVersion() == 3);
            REQUIRE(memento.getTimestamp() == "2025-01-15T10:30:00Z");
            REQUIRE(memento.getData("name") == "LoadedPlayer");
            REQUIRE(memento.getData("level") == 10);
        }

        SECTION("fromJson handles missing version") {
            nlohmann::json json = {
                {"timestamp", "2025-01-01T12:00:00Z"},
                {"data", {{"key", "value"}}}
            };

            BaseMemento memento;
            memento.setVersion(5);
            memento.fromJson(json);

            REQUIRE(memento.getVersion() == 5); // Unchanged
        }

        SECTION("fromJson handles missing timestamp") {
            nlohmann::json json = {
                {"version", 2},
                {"data", {{"key", "value"}}}
            };

            BaseMemento memento;
            memento.setTimestamp("original");
            memento.fromJson(json);

            REQUIRE(memento.getTimestamp() == "original"); // Unchanged
        }

        SECTION("fromJson handles missing data") {
            nlohmann::json json = {
                {"version", 2},
                {"timestamp", "2025-01-01T12:00:00Z"}
            };

            BaseMemento memento;
            memento.setData("existing", "value");
            memento.fromJson(json);

            REQUIRE(memento.getVersion() == 2);
            REQUIRE(memento.getTimestamp() == "2025-01-01T12:00:00Z");
        }
    }

    SECTION("Validation") {
        SECTION("isValid returns false for default memento") {
            BaseMemento memento;
            REQUIRE_FALSE(memento.isValid());
        }

        SECTION("isValid returns false with only version") {
            BaseMemento memento;
            memento.setVersion(1);
            REQUIRE_FALSE(memento.isValid());
        }

        SECTION("isValid returns false with only timestamp") {
            BaseMemento memento;
            memento.setVersion(0);
            memento.setTimestamp("2025-01-01T12:00:00Z");
            REQUIRE_FALSE(memento.isValid());
        }

        SECTION("isValid returns true with both version and timestamp") {
            BaseMemento memento;
            memento.setVersion(1);
            memento.setTimestamp("2025-01-01T12:00:00Z");
            REQUIRE(memento.isValid());
        }

        SECTION("isValid returns false with zero version") {
            BaseMemento memento;
            memento.setVersion(0);
            memento.setTimestamp("2025-01-01T12:00:00Z");
            REQUIRE_FALSE(memento.isValid());
        }
    }

    SECTION("Round-trip Serialization") {
        SECTION("Serialize and deserialize preserves all data") {
            BaseMemento original;
            original.setVersion(5);
            original.setTimestamp("2025-01-20T15:45:00Z");
            original.setData("playerName", "Hero");
            original.setData("score", 9999);
            original.setData("inventory", nlohmann::json::array({"sword", "shield"}));
            original.setData("position", nlohmann::json::object({{"x", 10.5}, {"y", 20.3}}));

            auto json = original.toJson();

            BaseMemento restored;
            restored.fromJson(json);

            REQUIRE(restored.getVersion() == 5);
            REQUIRE(restored.getTimestamp() == "2025-01-20T15:45:00Z");
            REQUIRE(restored.getData("playerName") == "Hero");
            REQUIRE(restored.getData("score") == 9999);
            REQUIRE(restored.getData("inventory").size() == 2);
            REQUIRE(restored.getData("position")["x"] == 10.5);
        }
    }
}
