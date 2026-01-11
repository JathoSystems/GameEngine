#ifndef GAMEENGINE_ANIMATOR_H
#define GAMEENGINE_ANIMATOR_H
#include "Spritesheet.h"
#include "../Component/Component.h"
#include <memory>

class Animator : public Component {
private:
    float _frameTime = 0.1f;
    float _accumulator = 0.0f;

    int _currentFrame = 0;
    int _rows;
    int _cols;
    int _min = 0;
    int _max = 36;
    std::unique_ptr<SpriteSheet> _sheet;

public:
    Animator(std::string path, int rows, int cols);

    void update(float deltaTime) override;

    void render(const std::unique_ptr<Window> &window) override;

    void setMin(int min);

    void setMax(int max);

    int getMin() const;

    int getMax() const;

    int getTotalFrames() const;
};


#endif //GAMEENGINE_ANIMATOR_H
