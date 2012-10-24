#include <global.h>

#include "ShadowCubeMapArray.h"
#include "Vec3.h"
#include "RenderEffect.h"

const Vec3 ShadowCubeMapArray::getFaceDirection(int i) {
    static const Vec3 faces[] = {
         Vec3::UNIT_X,
        -Vec3::UNIT_X,
        -Vec3::UNIT_Y,  // This is a quirk to for proper cubemap sampling.
         Vec3::UNIT_Y,  // For some reason cubemaps have Y flipped.
         Vec3::UNIT_Z,
        -Vec3::UNIT_Z,
    };

    return faces[i];
}

ShadowCubeMapArray::ShadowCubeMapArray() {
    this->width = 512;
    this->height = 512;
    
    this->initialize();
}

ShadowCubeMapArray::ShadowCubeMapArray(int dimension) {
    this->width = dimension;
    this->height = dimension;
    
    this->initialize();
}

void ShadowCubeMapArray::setDimension(int dimension) {
    this->width = dimension;
    this->height = dimension;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->texture);
    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_R16F,
            this->width, this->height, RenderEffect::MAX_LIGHTS * 6,
            0, GL_RED, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}

void ShadowCubeMapArray::initialize() {
    this->type = GL_TEXTURE_CUBE_MAP_ARRAY;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_R16F,
            this->width, this->height, RenderEffect::MAX_LIGHTS * 6,
            0, GL_RED, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}
