#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "ImageTexture.h"
#include "Image.h"

ImageTexture::ImageTexture() {
    this->anisotropyLevel = 0;
    
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageTexture::setAnisotropyLevel(int anisotropyLevel) {
    this->anisotropyLevel = anisotropyLevel;

// JFI
//    if (ANISOTROPY > 1 && Extention::isSupported("GL_EXT_texture_filter_anisotropic")) {
//        GLfloat maxAnisotropy;
//        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
//        maxAnisotropy = (ANISOTROPY > maxAnisotropy) ? maxAnisotropy : ANISOTROPY;
//        glSamplerParameteri(this->sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
//    }
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyLevel);
    glBindTexture(GL_TEXTURE_2D, 0);  
}

bool ImageTexture::load(const Image& image) {
    if (!image.isNull()) {
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, image.getWidth(), image.getHeight(),
                0, image.getFormat(), GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }
    
    return false;
}
