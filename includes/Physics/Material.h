#ifndef GAMEENGINE_MATERIAL_H
#define GAMEENGINE_MATERIAL_H

class Material {
public:
    float density;
    float friction;
    float restitution;

    Material(float density = 1.0f, float friction = 0.4f, float restitution = 0.0f)
        : density(density), friction(friction), restitution(restitution) {
    }

    float getDensity() const { return density; }
    float getFriction() const { return friction; }
    float getRestitution() const { return restitution; }
};

#endif
