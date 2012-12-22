#include "Texture.h"

Texture::Texture() {
    glGenTextures(1, &this->texture);

    this->type = 0;         // Should be overriden in subclasses
    this->width = 0;
    this->height = 0;
}

Texture::~Texture() {
    glDeleteTextures(1, &this->texture);
}

void Texture::bind(int target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(this->type, this->texture);
}
