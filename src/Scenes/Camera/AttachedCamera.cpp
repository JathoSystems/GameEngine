#include "Scenes/Camera/AttachedCamera.h"

AttachedCamera::AttachedCamera(std::unique_ptr<Viewport> viewport, GameObject* following)
    : Camera(std::move(viewport)), _following(following) {
}

void AttachedCamera::switchAttachedObject(GameObject* object) {
    _following = object;
}

Position AttachedCamera::getPosition() const {
    Position* pos = _following->getTransform()->getPosition();
    if (pos) {
        return *pos;  // Dereference to copy
    }
    return Position{0, 0};  // Default position if no object
}

GameObject* AttachedCamera::getFollowing() const {
    return _following;
}