#ifndef GAMEENGINE_ANIMATION_HPP
#define GAMEENGINE_ANIMATION_HPP
#include <map>
#include "Keyframe.hpp"
#include "Converter/IConverter.hpp"
#include "GameObjects/Component/Component.h"

enum class AnimationType {
    LERP, EIEO, BOUNCE, ELASTIC
};

class Animation : public Component {
private:
    float _time = 0;
    std::unique_ptr<IConverter> _converter;
    std::map<float, std::unique_ptr<Keyframe> > _keyFrames;

    float getMaxTime() const;

public:
    Animation(AnimationType type = AnimationType::EIEO);

    void addKeyframe(float time, std::unique_ptr<Keyframe> frame);

    void update(float deltaTime) override;

    void render(const std::unique_ptr<Window> &window) override;
};

#endif //GAMEENGINE_ANIMATION_HPP
