#pragma once
#include "Input/IKeyListener.h"
#include <vector>

class MockKeyListener : public IKeyListener {
public:
    std::vector<Key> pressedKeys;
    std::vector<Key> releasedKeys;

    void onKeyPress(Key key) override {
        pressedKeys.push_back(key);
    }

    void onKeyRelease(Key key) override {
        releasedKeys.push_back(key);
    }
};
