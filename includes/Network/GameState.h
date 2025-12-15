#pragma once

#include <string>

// Lightweight global network/game state shared inside the engine and game.
// Used here to gate player input until the server signals that the game is ready,
// and to expose the locally assigned player role (e.g. "fireboy", "watergirl").
class GameNetworkState {
public:
    static void setReady(bool ready);
    static bool isReady();

    static void setLocalRole(const std::string& role);
    static std::string getLocalRole();
};


