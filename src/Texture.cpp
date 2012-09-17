#include "Texture.h"

Texture::Texture() {
    glGenTextures(1, &this->texture);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->texture);
}

void Texture::bind(int target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}
