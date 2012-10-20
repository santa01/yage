#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "ImageTexture.h"
#include "Image.h"

ImageTexture::ImageTexture() {
    this->anisotropyLevel = 0;
    this->initialize();
}

ImageTexture::ImageTexture(int anisotropyLevel) {
    this->anisotropyLevel = anisotropyLevel;
    this->initialize();
}

bool ImageTexture::load(const Image& image) {
    if (!image.isNull()) {
        this->width = image.getWidth();
        this->height = image.getHeight();
        
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, this->width, this->height,
                0, image.getFormat(), GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return true;
    }
    
    return false;
}

// JFI
// if (ANISOTROPY > 1 && Extention::isSupported("GL_EXT_texture_filter_anisotropic")) {
//     GLfloat maxAnisotropy;
//     glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
//     maxAnisotropy = (ANISOTROPY > maxAnisotropy) ? maxAnisotropy : ANISOTROPY;
//     glSamplerParameteri(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
// }

void ImageTexture::initialize() {
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (this->anisotropyLevel > 0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisotropyLevel);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
