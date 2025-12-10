#ifndef GAMEENGINE_RIGIDBODY_H
#define GAMEENGINE_RIGIDBODY_H

#include "BodyType.h"
#include "Material.h"
#include <memory>

class RigidBody {
private:
    BodyType _bodyType;
    Material _material;
    float _gravityScale;
    bool _fixedRotation;
    
public:
    explicit RigidBody(BodyType type = BodyType::DYNAMIC)
        : _bodyType(type), _material(Material()), _gravityScale(1.0f), _fixedRotation(false) {}
    
    BodyType getBodyType() const { return _bodyType; }
    void setBodyType(BodyType type) { _bodyType = type; }
    
    Material& getMaterial() { return _material; }
    const Material& getMaterial() const { return _material; }
    void setMaterial(const Material& material) { _material = material; }
    
    float getGravityScale() const { return _gravityScale; }
    void setGravityScale(float scale) { _gravityScale = scale; }
    
    bool isFixedRotation() const { return _fixedRotation; }
    void setFixedRotation(bool fixed) { _fixedRotation = fixed; }
};

#endif //GAMEENGINE_RIGIDBODY_H
