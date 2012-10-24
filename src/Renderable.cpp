#include "Renderable.h"

Renderable::Renderable() {
    this->diffuseTexture = NULL;
    this->effect = NULL;
    this->shadowCaster = false;
    this->shadowReceiver = false;
}

Renderable::~Renderable() {
}
