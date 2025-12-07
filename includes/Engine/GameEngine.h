#ifndef GAMEENGINE_GAMEENGINE_H
#define GAMEENGINE_GAMEENGINE_H
#include <memory>
#include <vector>

#include "ISystem.h"
#include "Audio/AudioSystem.h"
#include "SDL/Window.h"

class GameEngine {
private:
	bool _isRunning;
	std::vector<std::unique_ptr<ISystem>> _systems;
	std::unique_ptr<Window> _window;
    std::unique_ptr<AudioSystem> _audioSystem;

public:
	GameEngine();

	void init(std::string name, int width, int height);
	void start();
	void stop();
	const std::unique_ptr<Window>& getWindow() const;
	AudioSystem* getAudioSystem() const { return _audioSystem.get(); }

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