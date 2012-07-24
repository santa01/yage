#include <memory.h>
#include <cmath>

#include "SpotLight.h"
#include "Light.h"
#include "Quaternion.h"
#include "Vec3.h"

SpotLight::SpotLight():
        direction(0.0f, -1.0f, 0.0f) {
    this->type = Light::TYPE_SPOT;
    this->falloff = 30.0f;
    this->size = 45.0f;
    this->blend = 0.15f;
}

SpotLight::~SpotLight() {
}

float SpotLight::getXAngle() const {
    // TODO
}

float SpotLight::getYAngle() const {
    // TODO
}

float SpotLight::getZAngle() const {
    // TODO
}

void SpotLight::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * M_PI / 180.0f);
    q.normalize();
    
    this->direction = q.extractMat4().extractMat3() * this->direction;
}

LightData SpotLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    data.size = this->size;
    data.falloff = this->falloff;
    data.blend = this->blend;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.position, this->position.data(), sizeof(data.position));
    memcpy(data.direction, this->direction.data(), sizeof(data.direction));
    
    return data;
}
