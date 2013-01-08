#include <global.h>

#include "ImageTexture.h"
#include "Image.h"

ImageTexture::ImageTexture() {
    this->anisotropyLevel = 0;
    this->initialize();
}

ImageTexture::ImageTexture(float anisotropyLevel) {
    this->anisotropyLevel = anisotropyLevel;
    this->initialize();
}

void ImageTexture::setAnisotropyLevel(float anisotropyLevel) {
    if (anisotropyLevel > 0.0f) {
        this->anisotropyLevel = anisotropyLevel;

        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisotropyLevel);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
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

void ImageTexture::initialize() {
    this->type = GL_TEXTURE_2D;

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (this->anisotropyLevel > 0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisotropyLevel);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}
