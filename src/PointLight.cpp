#include <memory.h>

#include "PointLight.h"
#include "Light.h"
#include "PerspectiveProjection.h"
#include "configuration.h"

PointLight::PointLight() {
    this->type = Light::TYPE_POINT;
    this->falloff = 30.0f;
    
    this->shadowMap.setDimention(SHADOW);
    
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
    data.shadow = this->shadow;
    memcpy(data.color, this->color.data(), sizeof(data.color));
    memcpy(data.position, this->position.data(), sizeof(data.position));
    
    return data;
}
