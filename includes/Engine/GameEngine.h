#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H
#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>

#include "ISystem.h"
#include "TimeManager.h"
#include "Audio/AudioSystem.h"
#include "SDL/Window.h"

class GameEngine {
private:
	bool _isRunning;
	std::vector<std::unique_ptr<ISystem>> _systems;
	std::unique_ptr<Window> _window;
	std::unique_ptr<TimeManager> _timeManager;

	GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
public:

	static GameEngine& getInstance();

	void init(std::string name, int width, int height);
	void start();
	void stop();
	const std::unique_ptr<Window>& getWindow() const;
	TimeManager* getTimeManager() const { return _timeManager.get(); }

	template<typename T>
	void addSystem(std::unique_ptr<T> system) {
		static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem");
		_systems.push_back(std::move(system));
	}

	template<typename T>  T* getSystem() {
		for (auto& system : _systems) {
			if (T* casted = dynamic_cast<T*>(system.get())) {
				return casted;
			}
		}
		return nullptr;
	}
};

#endif //GAMEENGINE_GAMEENGINE_H