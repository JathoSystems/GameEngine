#ifndef GAMEENGINE_MATERIAL_H
#define GAMEENGINE_MATERIAL_H

class Material {
private:
    float _density;
    float _friction;
    float _restitution; // Bounciness (0 = no bounce, 1 = perfect bounce)

public:
    Material(float density = 1.0f, float friction = 0.3f, float restitution = 0.0f);
    
    float getDensity() const { return _density; }
    float getFriction() const { return _friction; }
    float getRestitution() const { return _restitution; }
    
    void setDensity(float density) { _density = density; }
    void setFriction(float friction) { _friction = friction; }
    void setRestitution(float restitution) { _restitution = restitution; }
    
    // Preset materials
    static Material Wood();
    static Material Metal();
    static Material Rubber();
    static Material Ice();
};

#endif //GAMEENGINE_MATERIAL_H
