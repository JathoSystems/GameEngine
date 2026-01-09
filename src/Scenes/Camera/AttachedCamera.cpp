#include "Scenes/Camera/AttachedCamera.h"


AttachedCamera::AttachedCamera(std::unique_ptr<Viewport> viewport, GameObject* following)
    : Camera(std::move(viewport)), _following(following) {
}

void AttachedCamera::switchAttachedObject(GameObject* object) {
    _following = object;
}

Position AttachedCamera::getPosition() const {
    if (!_following) {
        return Position{0, 0};
    }

    Transform* transform = _following->getTransform();
    if (!transform) {
        return Position{0, 0};
    }

    Position* pos = transform->getPosition();
    if (!pos) {
        return Position{0, 0};
    }
    return *pos;
}

GameObject* AttachedCamera::getFollowing() const {
    return _following;
}