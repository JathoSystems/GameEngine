#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "Events/EventManager.h"
#include "Events/EventRegistry.h"
#include "Events/IEvent.h"
#include "GameObjects/GameObject.h"

class ButtonClickEvent : public IEvent {
private:
    std::string buttonName;
    int clickCount;

public:
    ButtonClickEvent(const std::string& btnName, int count)
        : buttonName(btnName), clickCount(count) {
        name = "ButtonClickEvent";
    }

    std::string getName() const override { return name; }

    Package serialize() const override {
        Package package;
        uint32_t count = static_cast<uint32_t>(clickCount);
        const uint8_t* countPointer = reinterpret_cast<const uint8_t*>(&count);
        package.insert(package.end(), countPointer, countPointer + sizeof(count));
        return package;
    }

    Data deserialize(const Package& package) const override {
        return package;
    }

    void apply(std::shared_ptr<GameObject> gameObject) override {
        std::cout << "Applying ButtonClickEvent: " << buttonName
                  << " clicked " << clickCount << " times" << std::endl;
    }

    const std::string& getButtonName() const { return buttonName; }
    int getClickCount() const { return clickCount; }
};

class PlayerMoveEvent : public IEvent {
private:
    float positionX;
    float positionY;

public:
    PlayerMoveEvent(float posX, float posY)
        : positionX(posX), positionY(posY) {
        name = "PlayerMoveEvent";
    }

    std::string getName() const override { return name; }

    Package serialize() const override {
        Package package;
        const uint8_t* xPointer = reinterpret_cast<const uint8_t*>(&positionX);
        const uint8_t* yPointer = reinterpret_cast<const uint8_t*>(&positionY);
        package.insert(package.end(), xPointer, xPointer + sizeof(positionX));
        package.insert(package.end(), yPointer, yPointer + sizeof(positionY));
        return package;
    }

    Data deserialize(const Package& package) const override {
        return package;
    }

    void apply(std::shared_ptr<GameObject> gameObject) override {
        std::cout << "Applying PlayerMoveEvent: Moving to position ("
                  << positionX << ", " << positionY << ")" << std::endl;
    }

    float getX() const { return positionX; }
    float getY() const { return positionY; }
};

int main() {
    try {
        auto gameEngine = std::make_unique<GameEngine>();
        gameEngine->init("Event System Demo", 800, 600);

        auto eventManager = std::make_unique<EventManager>(nullptr);
        EventRegistry* eventRegistry = EventRegistry::getInstance();

        eventRegistry->registerEvent("ButtonClickEvent", []() {
            return std::make_shared<ButtonClickEvent>("PlayButton", 1);
        });

        eventRegistry->registerEvent("PlayerMoveEvent", []() {
            return std::make_shared<PlayerMoveEvent>(100.0f, 250.0f);
        });

        eventRegistry->createEvent("ButtonClickEvent");
        auto buttonClickEvent = eventRegistry->getEvent("ButtonClickEvent");
        if (buttonClickEvent) {
            std::cout << "Broadcasting: " << buttonClickEvent->getName() << std::endl;
            buttonClickEvent->apply(nullptr);
            eventManager->broadcast(buttonClickEvent);
        }

        std::cout << std::endl;

        eventRegistry->createEvent("PlayerMoveEvent");
        auto playerMoveEvent = eventRegistry->getEvent("PlayerMoveEvent");
        if (playerMoveEvent) {
            std::cout << "Broadcasting: " << playerMoveEvent->getName() << std::endl;
            playerMoveEvent->apply(nullptr);
            eventManager->broadcast(playerMoveEvent);
        }

        gameEngine->start();

    } catch (const std::exception &exception) {
        std::cerr << "Exception: " << exception.what() << std::endl;
    }

    return 0;
}
