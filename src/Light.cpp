#include <cmath>

#include "Light.h"
#include "Quaternion.h"

Light::Light():
        color(1.0f, 1.0f, 1.0f),
        direction(0.0f, -1.0f, 0.0f) {
    this->energy = 1.0f;
    this->shadowCaster = false;
    this->valid = true;
    this->raw = true;
}

Light::~Light() {
}

float Light::getXAngle() const {
    // TODO
    return 0.0f;
}

float Light::getYAngle() const {
    // TODO
    return 0.0f;
}

float Light::getZAngle() const {
    // TODO
    return 0.0f;
}

void Light::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * M_PI / 180.0f);
    q.normalize();
    
    this->direction = q.extractMat4().extractMat3() * this->direction;
    this->valid = false;
}
