#pragma once
#include "Input/IMouseListener.h"
#include <vector>
#include <memory>

class MockMouseListener : public IMouseListener {
public:
    std::vector<std::unique_ptr<Position> > movedPositions;
    std::vector<MouseButton> clickedButtons;
    std::vector<MouseButton> pressedButtons;
    std::vector<MouseButton> releasedButtons;

    void onMouseMoved(std::unique_ptr<Position> position) override {
        movedPositions.push_back(std::move(position));
    }

    void onMouseClicked(MouseButton button) override {
        clickedButtons.push_back(button);
    }

    void onMousePressed(MouseButton button) override {
        pressedButtons.push_back(button);
    }

    void onMouseReleased(MouseButton button) override {
        releasedButtons.push_back(button);
    }
};
