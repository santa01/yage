#include "Renderable.h"

Renderable::Renderable() {
    this->diffuseTexture = NULL;
    this->effect = NULL;
    this->castsShadow = false;
    this->receivesShadow = false;
}

Renderable::~Renderable() {
}
