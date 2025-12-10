#include "Engine/GameEngine.h"
#include "Scenes/Scene.h"
#include "Scenes/Camera/FixedCamera.h"
#include "UI/HUD.h"
#include "UI/FPSCounter.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Component/KeyInputComponent.h"
#include "Input/IKeyListener.h"
#include <memory>

class FPSToggleListener : public IKeyListener {
private:
    FPSCounter* _fpsCounter;

public:
    explicit FPSToggleListener(FPSCounter* fpsCounter) : _fpsCounter(fpsCounter) {}

    void onKeyPress(Key key) override {
        if (key == Key::F3) {
            _fpsCounter->toggleVisibility();
        }
    }

    void onKeyRelease(Key key) override {}
};

int main() {
    GameEngine engine;
    engine.init("FPS Counter Demo", 800, 600);

    auto scene = std::make_unique<Scene>("MainScene");

    auto viewport = std::make_unique<Viewport>(
        Size(800, 600),
        Position(0, 0)
    );
    auto camera = std::make_unique<FixedCamera>(std::move(viewport), Position(400, 300));
    scene->setCamera(std::move(camera));

    auto hud = std::make_unique<HUD>();

    auto fpsCounter = std::make_unique<FPSCounter>();
    fpsCounter->setPosition(700.0f, 10.0f);
    fpsCounter->setSize(100.0f, 30.0f);
    fpsCounter->setFontSize(20);

    hud->setFPSCounter(std::move(fpsCounter));

    auto inputObj = std::make_unique<GameObject>();
    auto toggleListener = std::make_shared<FPSToggleListener>(hud->getFPSCounter());
    auto keyInput = std::make_unique<KeyInputComponent>(inputObj.get());
    keyInput->setListener(toggleListener.get());

    engine.getInputSystem()->registerKeyComponent(keyInput.get());

    inputObj->addComponent(std::move(keyInput));
    scene->addObject(std::move(inputObj));

    scene->setHUD(std::move(hud));

    engine.getSceneManager()->addScene(std::move(scene));
    engine.getSceneManager()->setScene("MainScene");

    engine.start();

    return 0;
}
