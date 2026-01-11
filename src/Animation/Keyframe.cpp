#include "Animation/Keyframe.hpp"

Keyframe::Keyframe(std::unique_ptr<Transform> transform) {
    _target = std::move(transform);
}

void Keyframe::transition(IConverter *converter, Transform *prev, Transform *target, GameObject *parent, float time) {
    Transform *currentTransform = parent->getTransform();

    Position *currentPosition = currentTransform->getPosition();
    Position *beginPosition = prev->getPosition();
    Position *targetPosition = target->getPosition();
    currentPosition->setX(converter->convert(beginPosition->getX(), targetPosition->getX(), time));
    currentPosition->setY(converter->convert(beginPosition->getY(), targetPosition->getY(), time));

    Size *currentSize = currentTransform->getSize();
    Size *beginSize = prev->getSize();
    Size *targetSize = target->getSize();
    if (beginSize->getWidth() != targetSize->getWidth()) {
        currentSize->setWidth(converter->convert(beginSize->getWidth(), targetSize->getWidth(), time));
    }
    if (beginSize->getHeight() != targetSize->getHeight()) {
        currentSize->setHeight(converter->convert(beginSize->getHeight(), targetSize->getHeight(), time));
    }

    Scale *currentScale = currentTransform->getScale();
    Scale *beginScale = prev->getScale();
    Scale *targetScale = target->getScale();
    if (beginScale->getScale() != targetScale->getScale()) {
        currentScale->setScale(converter->convert(beginScale->getScale(), targetScale->getScale(), time));
    }

    Rotation *currentRotation = currentTransform->getRotation();
    Rotation *beginRotation = prev->getRotation();
    Rotation *targetRotation = target->getRotation();
    if (beginRotation->getRotation() != targetRotation->getRotation()) {
        currentRotation->setRotation(converter->convert(beginRotation->getRotation(), targetRotation->getRotation(),
                                                        time));
    }
}

Transform *Keyframe::getTransform() {
    return _target.get();
}
