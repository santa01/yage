#include <memory.h>

#include "PointLight.h"

PointLight::PointLight() {
    this->type = Light::TYPE_POINT;
    this->falloff = 30.0f;
    
    this->shadowProjection = new PerspectiveProjection();
    this->shadowProjection->setAspectRatio(1.0f);
    this->shadowProjection->setFov(90.0f);
}

PointLight::~PointLight() {
    delete this->shadowProjection;
}

LightData PointLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    data.falloff = this->falloff;
    data.shadow = this->shadowCaster;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.position, this->position.data(), sizeof(data.position));
    
    return data;
}
