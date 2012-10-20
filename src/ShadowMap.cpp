#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "ShadowMap.h"
#include "Vec3.h"

ShadowMap::ShadowMap() {
    this->width = 512;
    this->height = 512;
    
    this->initialize();
}

ShadowMap::ShadowMap(int dimention) {
    this->width = dimention;
    this->height = dimention;
    
    this->initialize();
}

void ShadowMap::setDimention(int dimention) {
    this->width = dimention;
    this->height = dimention;
    
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
            this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMap::initialize() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
            this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}
