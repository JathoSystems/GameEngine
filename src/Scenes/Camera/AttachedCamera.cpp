#include "Scenes/Camera/AttachedCamera.h"

AttachedCamera::AttachedCamera(std::unique_ptr<Viewport> viewport, GameObject *following)
    : Camera(std::move(viewport)), _following(following) {
}

void AttachedCamera::switchAttachedObject(GameObject *object) {
    _following = object;
}

Position AttachedCamera::getPosition() const {
    if (!_following) {
        return Position{0, 0};
    }

    Position *pos = _following->getTransform()->getPosition();
    if (pos) {
        return *pos;
    }
    return Position{0, 0};
}

GameObject *AttachedCamera::getFollowing() const {
    return _following;
}
