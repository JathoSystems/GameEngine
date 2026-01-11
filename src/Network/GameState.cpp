#include "../../includes/Network/GameState.hpp"

GameState &GameState::getInstance() {
    static GameState instance;
    return instance;
}
