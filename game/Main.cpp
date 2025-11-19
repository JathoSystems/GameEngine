#include <iostream>
#include <memory>

#include "Engine/GameEngine.h"
#include "Events/EventManager.h"
#include "Events/EventRegistry.h"
#include "Events/IEvent.h"

// Inline ButtonClickEvent for demonstration
class ButtonClickEvent : public IEvent {
public:
    ButtonClickEvent(const std::string& btnName, int count)
        : buttonName(btnName), clickCount(count) {
        name = "ButtonClickEvent";
    }

    std::string getName() const override { return name; }

    Package serialize() const override {
        Package p;
        uint32_t cnt = static_cast<uint32_t>(clickCount);
        const uint8_t* cntPtr = reinterpret_cast<const uint8_t*>(&cnt);
        p.insert(p.end(), cntPtr, cntPtr + sizeof(cnt));
        return p;
    }

    Data deserialize(const Package& package) const override { return package; }
    void apply(GameObject* /*gameObject*/) override { /* no-op */ }

    const std::string& getButtonName() const { return buttonName; }
    int getClickCount() const { return clickCount; }

private:
    std::string buttonName;
    int clickCount;
};

int main() {
    try {
        std::unique_ptr<GameEngine> engine = std::make_unique<GameEngine>();
        engine->init("Event System Demo", 800, 600);

        // Initialize Event System
        EventManager* eventManager = new EventManager(nullptr);
        EventRegistry* registry = EventRegistry::getInstance();

        // Create and broadcast an event
        ButtonClickEvent* testEvent = new ButtonClickEvent("TestButton", 1);
        eventManager->broadcast(testEvent);
        std::cout << "Event broadcasted: " << testEvent->getName()
                  << " for button '" << testEvent->getButtonName()
                  << "' - Click #" << testEvent->getClickCount() << std::endl;
        delete testEvent;

        // Broadcast another event
        ButtonClickEvent* secondEvent = new ButtonClickEvent("AnotherButton", 2);
        eventManager->broadcast(secondEvent);
        std::cout << "Event broadcasted: " << secondEvent->getName()
                  << " for button '" << secondEvent->getButtonName()
                  << "' - Click #" << secondEvent->getClickCount() << std::endl;
        delete secondEvent;

        engine->start();

        delete eventManager;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}