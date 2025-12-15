#include "Network/GameState.h"

#include <string>

namespace {
    bool g_ready = false;
    std::string g_localRole;
}

void GameNetworkState::setReady(bool ready) {
    g_ready = ready;
}

bool GameNetworkState::isReady() {
    return g_ready;
}

void GameNetworkState::setLocalRole(const std::string& role) {
    g_localRole = role;
}

std::string GameNetworkState::getLocalRole() {
    return g_localRole;
}

