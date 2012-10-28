#include "Texture.h"
#include "Image.h"
#include "Extention.h"
#include "configuration.h"

Texture::Texture() {
    glGenTextures(1, &this->texture);

    glGenSamplers(1, &this->sampler);
    glSamplerParameterf(this->sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameterf(this->sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

Texture::~Texture() {
    glDeleteSamplers(1, &this->sampler);
    glDeleteTextures(1, &this->texture);
}

bool Texture::load(const Image& image) {
    if (!image.isNull()) {
        glBindTexture(GL_TEXTURE_2D, this->texture);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, image.getWidth(), image.getHeight(),
                0, image.getFormat(), GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        
        if (ANISOTROPY > 1 && Extention::isSupported("GL_EXT_texture_filter_anisotropic")) {
            GLfloat maxAnisotropy;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            maxAnisotropy = (ANISOTROPY > maxAnisotropy) ? maxAnisotropy : ANISOTROPY;
            glSamplerParameterf(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }
    
    return false;
}

void Texture::bind(int target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBindSampler(target, this->sampler);
}
