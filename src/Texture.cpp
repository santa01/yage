#include "Texture.h"

Texture::Texture() {
    glGenTextures(1, &this->texture);
    this->width = 0;
    this->height = 0;
}

Texture::~Texture() {
    glDeleteTextures(1, &this->texture);
}

void Texture::bind(int target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
