#include <catch2/catch_test_macros.hpp>
#include "GameObjects/Component/Component.h"
#include "SDL/Window.h"
#include "GameObjects/GameObject.h"

// Test Component implementation
class TestComponent : public Component {
private:
    int renderCallCount = 0;
    int updateCallCount = 0;
    float lastDeltaTime = 0.0f;
    GameObject *parentObject = nullptr;
    const Window *lastWindow = nullptr;

public:
    void render(const std::unique_ptr<Window> &window) override {
        renderCallCount++;
        lastWindow = window.get();
    }

    void update(float delta) override {
        updateCallCount++;
        lastDeltaTime = delta;
    }

    void setParent(GameObject *parent) override {
        parentObject = parent;
    }

    // Getters for verification
    int getRenderCallCount() const { return renderCallCount; }
    int getUpdateCallCount() const { return updateCallCount; }
    float getLastDeltaTime() const { return lastDeltaTime; }
    GameObject *getParent() const { return parentObject; }
    const Window *getLastWindow() const { return lastWindow; }

    void reset() {
        renderCallCount = 0;
        updateCallCount = 0;
        lastDeltaTime = 0.0f;
    }
};

// Ordered test component to verify execution order
class OrderedTestComponent : public Component {
private:
    static int executionOrder;
    int myRenderOrder = -1;
    int myUpdateOrder = -1;
    int id;

public:
    explicit OrderedTestComponent(int componentId) : id(componentId) {
    }

    void render(const std::unique_ptr<Window> &window) override {
        myRenderOrder = executionOrder++;
    }

    void update(float delta) override {
        myUpdateOrder = executionOrder++;
    }

    void setParent(GameObject *parent) override {
    }

    int getRenderOrder() const { return myRenderOrder; }
    int getUpdateOrder() const { return myUpdateOrder; }
    int getId() const { return id; }

    static void resetExecutionOrder() { executionOrder = 0; }
};

int OrderedTestComponent::executionOrder = 0;

// Simple Window implementation for testing
class TestWindow : public Window {
    // Add any necessary Window implementation
};

// ==================== addComponent Tests ====================

TEST_CASE("GameObject", "[GameObject][addComponent]") {
    GameObject gameObject;
    auto component = std::make_unique<TestComponent>();
    auto *componentPtr = component.get();

    gameObject.addComponent(std::move(component));

    REQUIRE(componentPtr->getParent() == &gameObject);

    SECTION("AddComponent stores component", "[GameObject][addComponent]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        // Verify component is stored by calling render
        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        gameObject.render(window);

        REQUIRE(componentPtr->getRenderCallCount() == 1);
    }

    SECTION("AddComponent handles multiple components", "[GameObject][addComponent]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<TestComponent>();
        auto component2 = std::make_unique<TestComponent>();
        auto component3 = std::make_unique<TestComponent>();

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();
        auto *ptr3 = component3.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));
        gameObject.addComponent(std::move(component3));

        REQUIRE(ptr1->getParent() == &gameObject);
        REQUIRE(ptr2->getParent() == &gameObject);
        REQUIRE(ptr3->getParent() == &gameObject);
    }

    SECTION("AddComponent moves ownership", "[GameObject][addComponent]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();

        gameObject.addComponent(std::move(component));

        // Verify that component is now nullptr (ownership transferred)
        REQUIRE(component == nullptr);
    }

    // ==================== render Tests ====================

    SECTION("Render calls render on all components", "[GameObject][render]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<TestComponent>();
        auto component2 = std::make_unique<TestComponent>();

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));

        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        gameObject.render(window);

        REQUIRE(ptr1->getRenderCallCount() == 1);
        REQUIRE(ptr2->getRenderCallCount() == 1);
    }

    SECTION("Render with no components does not crash", "[GameObject][render]")
    {
        GameObject gameObject;
        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        REQUIRE_NOTHROW(gameObject.render(window));
    }

    SECTION("Render passes correct window", "[GameObject][render]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        auto *windowPtr = window.get();

        gameObject.render(window);

        REQUIRE(componentPtr->getLastWindow() == windowPtr);
    }

    SECTION("Render renders components in order", "[GameObject][render]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<OrderedTestComponent>(1);
        auto component2 = std::make_unique<OrderedTestComponent>(2);
        auto component3 = std::make_unique<OrderedTestComponent>(3);

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();
        auto *ptr3 = component3.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));
        gameObject.addComponent(std::move(component3));

        OrderedTestComponent::resetExecutionOrder();

        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        gameObject.render(window);

        REQUIRE(ptr1->getRenderOrder() == 0);
        REQUIRE(ptr2->getRenderOrder() == 1);
        REQUIRE(ptr3->getRenderOrder() == 2);
    }

    SECTION("Render can be called multiple times", "[GameObject][render]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        gameObject.render(window);
        gameObject.render(window);
        gameObject.render(window);

        REQUIRE(componentPtr->getRenderCallCount() == 3);
    }

    SECTION("Update calls update on all components", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<TestComponent>();
        auto component2 = std::make_unique<TestComponent>();

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));

        float deltaTime = 0.016f;
        gameObject.update(deltaTime);

        REQUIRE(ptr1->getUpdateCallCount() == 1);
        REQUIRE(ptr2->getUpdateCallCount() == 1);
    }

    SECTION("Update with no components does not crash", "[GameObject][update]")
    {
        GameObject gameObject;

        REQUIRE_NOTHROW(gameObject.update(0.016f));
    }

    SECTION("Update passes correct delta time", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        float expectedDelta = 1.234f;
        gameObject.update(expectedDelta);

        REQUIRE(componentPtr->getLastDeltaTime() == expectedDelta);
    }

    SECTION("Update updates components in order", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<OrderedTestComponent>(1);
        auto component2 = std::make_unique<OrderedTestComponent>(2);
        auto component3 = std::make_unique<OrderedTestComponent>(3);

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();
        auto *ptr3 = component3.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));
        gameObject.addComponent(std::move(component3));

        OrderedTestComponent::resetExecutionOrder();

        gameObject.update(0.016f);

        REQUIRE(ptr1->getUpdateOrder() == 0);
        REQUIRE(ptr2->getUpdateOrder() == 1);
        REQUIRE(ptr3->getUpdateOrder() == 2);
    }

    SECTION("Update with zero delta", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));
        gameObject.update(0.0f);

        REQUIRE(componentPtr->getUpdateCallCount() == 1);
        REQUIRE(componentPtr->getLastDeltaTime() == 0.0f);
    }

    SECTION("Update with negative delta", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));
        gameObject.update(-0.5f);

        REQUIRE(componentPtr->getUpdateCallCount() == 1);
        REQUIRE(componentPtr->getLastDeltaTime() == -0.5f);
    }

    SECTION("Update can be called multiple times", "[GameObject][update]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        gameObject.update(0.016f);
        gameObject.update(0.032f);
        gameObject.update(0.048f);

        REQUIRE(componentPtr->getUpdateCallCount() == 3);
        REQUIRE(componentPtr->getLastDeltaTime() == 0.048f);
    }

    SECTION("SetLayer changes layer value", "[GameObject][layer]")
    {
        GameObject gameObject;

        gameObject.setLayer(5);

        REQUIRE(gameObject.getLayer() == 5);
    }

    SECTION("GetLayer returns default value of one", "[GameObject][layer]")
    {
        GameObject gameObject;

        REQUIRE(gameObject.getLayer() == 1);
    }

    SECTION("SetLayer with negative value", "[GameObject][layer]")
    {
        GameObject gameObject;

        gameObject.setLayer(-3);

        REQUIRE(gameObject.getLayer() == -3);
    }

    SECTION("SetLayer with zero", "[GameObject][layer]")
    {
        GameObject gameObject;

        gameObject.setLayer(0);

        REQUIRE(gameObject.getLayer() == 0);
    }

    SECTION("SetLayer multiple times", "[GameObject][layer]")
    {
        GameObject gameObject;

        gameObject.setLayer(2);
        REQUIRE(gameObject.getLayer() == 2);

        gameObject.setLayer(10);
        REQUIRE(gameObject.getLayer() == 10);

        gameObject.setLayer(-5);
        REQUIRE(gameObject.getLayer() == -5);
    }

    SECTION("SetLayer with large values", "[GameObject][layer]")
    {
        GameObject gameObject;

        gameObject.setLayer(999999);
        REQUIRE(gameObject.getLayer() == 999999);

        gameObject.setLayer(-999999);
        REQUIRE(gameObject.getLayer() == -999999);
    }

    SECTION("GetTransform returns non-null", "[GameObject][transform]")
    {
        GameObject gameObject;

        Transform *transform = gameObject.getTransform();

        REQUIRE(transform != nullptr);
    }

    SECTION("GetTransform returns same instance", "[GameObject][transform]")
    {
        GameObject gameObject;

        Transform *transform1 = gameObject.getTransform();
        Transform *transform2 = gameObject.getTransform();

        REQUIRE(transform1 == transform2);
    }

    SECTION("GameObject integration: add components, update and render", "[GameObject][integration]")
    {
        GameObject gameObject;
        auto component1 = std::make_unique<TestComponent>();
        auto component2 = std::make_unique<TestComponent>();

        auto *ptr1 = component1.get();
        auto *ptr2 = component2.get();

        gameObject.addComponent(std::move(component1));
        gameObject.addComponent(std::move(component2));

        // Update cycle
        gameObject.update(0.016f);
        REQUIRE(ptr1->getUpdateCallCount() == 1);
        REQUIRE(ptr2->getUpdateCallCount() == 1);
        REQUIRE(ptr1->getLastDeltaTime() == 0.016f);
        REQUIRE(ptr2->getLastDeltaTime() == 0.016f);

        // Render cycle
        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        gameObject.render(window);
        REQUIRE(ptr1->getRenderCallCount() == 1);
        REQUIRE(ptr2->getRenderCallCount() == 1);
    }

    SECTION("GameObject integration: multiple update/render cycles", "[GameObject][integration]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *componentPtr = component.get();

        gameObject.addComponent(std::move(component));

        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();

        for (int i = 0; i < 5; ++i) {
            gameObject.update(0.016f);
            gameObject.render(window);
        }

        REQUIRE(componentPtr->getUpdateCallCount() == 5);
        REQUIRE(componentPtr->getRenderCallCount() == 5);
    }

    SECTION("GameObject integration: layer management with components", "[GameObject][integration]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();

        gameObject.addComponent(std::move(component));

        REQUIRE(gameObject.getLayer() == 1);

        gameObject.setLayer(5);
        REQUIRE(gameObject.getLayer() == 5);

        // Components should still work after layer change
        const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
        REQUIRE_NOTHROW(gameObject.render(window));
        REQUIRE_NOTHROW(gameObject.update(0.016f));
    }

    SECTION("GameObject component management", "[GameObject]")
    {
        GameObject gameObject;

        SECTION("Adding single component") {
            auto component = std::make_unique<TestComponent>();
            auto *ptr = component.get();

            gameObject.addComponent(std::move(component));

            REQUIRE(ptr->getParent() == &gameObject);
        }

        SECTION("Adding multiple components") {
            auto comp1 = std::make_unique<TestComponent>();
            auto comp2 = std::make_unique<TestComponent>();

            auto *ptr1 = comp1.get();
            auto *ptr2 = comp2.get();

            gameObject.addComponent(std::move(comp1));
            gameObject.addComponent(std::move(comp2));

            REQUIRE(ptr1->getParent() == &gameObject);
            REQUIRE(ptr2->getParent() == &gameObject);
        }

        SECTION("Components maintain state between calls") {
            auto component = std::make_unique<TestComponent>();
            auto *ptr = component.get();

            gameObject.addComponent(std::move(component));

            gameObject.update(0.5f);
            REQUIRE(ptr->getLastDeltaTime() == 0.5f);

            gameObject.update(1.0f);
            REQUIRE(ptr->getLastDeltaTime() == 1.0f);
        }
    }

    SECTION("GameObject lifecycle methods", "[GameObject]")
    {
        GameObject gameObject;
        auto component = std::make_unique<TestComponent>();
        auto *ptr = component.get();

        gameObject.addComponent(std::move(component));

        SECTION("Update is called correctly") {
            gameObject.update(0.016f);

            REQUIRE(ptr->getUpdateCallCount() == 1);
            REQUIRE(ptr->getLastDeltaTime() == 0.016f);
        }

        SECTION("Render is called correctly") {
            const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
            gameObject.render(window);

            REQUIRE(ptr->getRenderCallCount() == 1);
        }

        SECTION("Update and render work independently") {
            gameObject.update(0.016f);
            REQUIRE(ptr->getUpdateCallCount() == 1);
            REQUIRE(ptr->getRenderCallCount() == 0);

            const std::unique_ptr<Window> window = std::make_unique<TestWindow>();
            gameObject.render(window);
            REQUIRE(ptr->getUpdateCallCount() == 1);
            REQUIRE(ptr->getRenderCallCount() == 1);
        }
    }
}
