#include <global.h>

#include "ShadowMapArray.h"
#include "Vec3.h"
#include "RenderEffect.h"

ShadowMapArray::ShadowMapArray() {
    this->width = 512;
    this->height = 512;
    
    this->initialize();
}

ShadowMapArray::ShadowMapArray(int dimension) {
    this->width = dimension;
    this->height = dimension;
    
    this->initialize();
}

void ShadowMapArray::setDimension(int dimension) {
    this->width = dimension;
    this->height = dimension;
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
            this->width, this->height, RenderEffect::MAX_LIGHTS,
            0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ShadowMapArray::initialize() {
    this->type = GL_TEXTURE_2D_ARRAY;

    glBindTexture(GL_TEXTURE_2D_ARRAY, this->texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16,
            this->width, this->height, RenderEffect::MAX_LIGHTS,
            0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
