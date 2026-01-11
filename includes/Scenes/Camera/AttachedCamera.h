#pragma once
#include "Camera.h"
#include "GameObjects/GameObject.h"

class AttachedCamera : public Camera {
private:
    GameObject *_following;

public:
    AttachedCamera(std::unique_ptr<Viewport> viewport, GameObject *following);

    void switchAttachedObject(GameObject *object);

    Position getPosition() const override;

    GameObject *getFollowing() const;
};
