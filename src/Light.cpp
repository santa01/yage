#include <cmath>

#include "Light.h"
#include "Quaternion.h"
#include "Vec3.h"

Light::Light():
        color(1.0f, 1.0f, 1.0f),
        direction(0.0f, -1.0f, 0.0f) {
    this->energy = 1.0f;
    this->shadow = true;
    this->valid = true;
    this->raw = true;
}

Light::~Light() {
}

float Light::getXAngle() const {
    // TODO
}

float Light::getYAngle() const {
    // TODO
}

float Light::getZAngle() const {
    // TODO
}

void Light::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * M_PI / 180.0f);
    q.normalize();
    
    this->direction = q.extractMat4().extractMat3() * this->direction;
    this->setValid(false);
}
