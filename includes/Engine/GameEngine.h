#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H
#include <memory>
#include <vector>

#include "ISystem.h"
#include "Audio/AudioSystem.h"
#include "SDL/Window.h"
#include "Physics/PhysicsSystem.h"
#include "Input/InputSystem.h"
#include "Scenes/SceneManager.h"

class GameEngine {
private:
	bool _isRunning = false;
	std::vector<std::unique_ptr<ISystem>> _systems;
	std::unique_ptr<Window> _window;
	std::unique_ptr<AudioSystem> _audioSystem;
	std::unique_ptr<PhysicsSystem> _physicsSystem;
	std::unique_ptr<SceneManager> _sceneManager;

public:
	GameEngine();

	void init(std::string name, int width, int height);
	void start();
	void stop();
	const std::unique_ptr<Window>& getWindow() const;
	AudioSystem* getAudioSystem() const { return _audioSystem.get(); }
	PhysicsSystem* getPhysicsSystem() { return _physicsSystem.get(); }
	SceneManager* getSceneManager() { return _sceneManager.get(); }
	InputSystem* getInputSystem();

	template<typename T> T* getSystem() {
		for (auto& system : _systems) {
			if (T* casted = dynamic_cast<T*>(system.get())) {
				return casted;
			}
		}
		return nullptr;
	}
};

#endif
