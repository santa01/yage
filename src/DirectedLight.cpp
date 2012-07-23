#include <memory.h>

#include "DirectedLight.h"
#include "Light.h"
#include "Quaternion.h"
#include "Vec3.h"

DirectedLight::DirectedLight():
        direction(0.0f, -1.0f, 0.0f) {
    this->type = Light::TYPE_DIRECTED;
}

DirectedLight::~DirectedLight() {
}

float DirectedLight::getXAngle() const {
    // TODO
}

float DirectedLight::getYAngle() const {
    // TODO
}

float DirectedLight::getZAngle() const {
    // TODO
}

void DirectedLight::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * 3.1415f / 180.0f);
    q.normalize();
    
    this->direction = q.extractMat4().extractMat3() * this->direction;
}

LightData DirectedLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.direction, this->direction.data(), sizeof(data.direction));
    
    return data;
}
