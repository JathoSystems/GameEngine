//
// Created by jusra on 16-12-2025.
//

#include "../../includes/Network/GameState.hpp"

GameState & GameState::getInstance() {
    static GameState instance;
    return instance;
}
