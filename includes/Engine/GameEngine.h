//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H
#include <memory>
#include <vector>

#include "ISystem.h"
#include "../Scenes/Scene.h"
#include "SDL/Window.h"

class GameEngine {
private:
	bool _isRunning;
	std::vector<std::unique_ptr<ISystem>> _systems;
	std::unique_ptr<Window> _window;

public:
	GameEngine();

	void init(std::string name, int width, int height);
	void start();
	void stop();

	void addScene(std::unique_ptr<Scene> scene);

	const std::unique_ptr<Window>& getWindow() const;
};

#endif //GAMEENGINE_GAMEENGINE_H