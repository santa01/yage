#include <memory.h>
#include <cstdlib>

#include "DirectedLight.h"

DirectedLight::DirectedLight() {
    this->type = Light::TYPE_DIRECTED;
    
    this->shadowProjection = new OrthographicProjection();
    this->shadowProjection->setAspectRatio(1.0f);
}

DirectedLight::~DirectedLight() {
    delete this->shadowProjection;
}

LightData DirectedLight::getLightData() const {
    LightData data;
    memset(&data, 0, sizeof(data));
    
    data.type = this->type;
    data.energy = this->energy;
    data.shadow = this->shadowCaster;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.direction, this->direction.data(), sizeof(data.direction));
    
    return data;
}
