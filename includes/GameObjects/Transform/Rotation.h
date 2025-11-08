//
// Created by kikker234 on 07-11-2025.
//

#ifndef GAMEENGINE_ROTATION_H
#define GAMEENGINE_ROTATION_H

class Rotation {
private:
    int _rotation;

public:
    Rotation(int rotation);

    int getRotation() const;
    void setRotation(int rotation);
};

#endif //GAMEENGINE_ROTATION_H