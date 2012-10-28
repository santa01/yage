#include "CubeTexture.h"
#include "Image.h"

CubeTexture::CubeTexture() {
    this->anisotropyLevel = 0;
    this->initialize();
}

CubeTexture::CubeTexture(float anisotropyLevel) {
    this->anisotropyLevel = anisotropyLevel;
    this->initialize();
}

void CubeTexture::setAnisotropyLevel(float anisotropyLevel) {
    if (anisotropyLevel > 0.0f) {
        this->anisotropyLevel = anisotropyLevel;

        glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                this->anisotropyLevel);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}

bool CubeTexture::loadFaceImage(int face, const Image& image) {
    if (!image.isNull()) {
        int newWidth = image.getWidth();
        int newHeight = image.getHeight();
        if ((this->width > 0 && newWidth != this->width) ||
            (this->height > 0 && newHeight != this->height)) {
            // First Image defines width and height for all others
            return false;
        }

        this->width = newWidth;
        this->height = newHeight;

        glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
        glTexImage2D(face, 0, GL_SRGB8_ALPHA8, this->width, this->height,
                0, image.getFormat(), GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return true;
    }

    return false;
}

void CubeTexture::initialize() {
    this->type = GL_TEXTURE_CUBE_MAP;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (this->anisotropyLevel > 0) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                this->anisotropyLevel);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
