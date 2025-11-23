// //
// // Created by kikker234 on 22-11-2025.
// //
// #include "AI/states/StateManager.hpp"
//
// void StateManager::changeState(std::shared_ptr<IState> newState) {
//     _currentState->onEnter();
//     _currentState = newState;
//     _currentState->onEnter();
// }
//
// void StateManager::setInitialState(std::shared_ptr<IState> state) {
//     _currentState = state;
//     _currentState->onEnter();
// }
//
// void StateManager::update(float deltaTime) {
//     if (!_currentState) return;
//     _currentState->onUpdate(deltaTime);
//     for (auto &t : _currentState->getTransitions()) {
//         if (t->shouldTransition()) {
//             changeState(t->getTargetState());
//             break;
//         }
//     }
// }
