#include "Light.h"
#include "Quaternion.h"

Light::Light():
        color(1.0f, 1.0f, 1.0f) {
    this->energy = 1.0f;
}

Light::~Light() {
}
