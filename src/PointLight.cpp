#include <memory.h>

#include "PointLight.h"
#include "Light.h"

PointLight::PointLight() {
    this->type = Light::TYPE_POINT;
    this->falloff = 30.0f;
}

PointLight::~PointLight() {
}

LightData PointLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    data.falloff = this->falloff;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.position, this->position.data(), sizeof(data.position));
    
    return data;
}
