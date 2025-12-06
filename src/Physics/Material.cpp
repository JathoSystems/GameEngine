#include "Physics/Material.h"

Material::Material(float density, float friction, float restitution)
    : _density(density), _friction(friction), _restitution(restitution) {}

Material Material::Wood() {
    return Material(0.6f, 0.4f, 0.1f);
}

Material Material::Metal() {
    return Material(1.2f, 0.2f, 0.3f);
}

Material Material::Rubber() {
    return Material(0.9f, 0.8f, 0.9f);
}

Material Material::Ice() {
    return Material(0.9f, 0.02f, 0.0f);
}
