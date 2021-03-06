#include <memory.h>

#include "SpotLight.h"

SpotLight::SpotLight() {
    this->type = Light::TYPE_SPOT;
    this->falloff = 30.0f;
    this->size = 45.0f;
    this->blend = 0.15f;
    
    this->shadowProjection = new PerspectiveProjection();
    this->shadowProjection->setAspectRatio(1.0f);
    this->shadowProjection->setFov(this->size);
}

SpotLight::~SpotLight() {
    delete this->shadowProjection;
}

LightData SpotLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    data.size = this->size;
    data.falloff = this->falloff;
    data.blend = this->blend;
    data.shadow = this->shadowCaster;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.position, this->position.data(), sizeof(data.position));
    memcpy(data.direction, this->direction.data(), sizeof(data.direction));
    
    return data;
}
