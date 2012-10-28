#include <global.h>

#include "SkyBox.h"
#include "Vec3.h"

const float SkyBox::vertices[] = {
    -100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,
    
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
    
     100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
    
    -100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f, -100.0f, -100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f, -100.0f,
    
    -100.0f,  100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f,  100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,
    
    -100.0f, -100.0f,  100.0f,
     100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
};

SkyBox::SkyBox() {
    this->initialize();
}

SkyBox::SkyBox(float x, float y, float z) {
    this->initialize();
    this->setPosition(x, y, z);
}

SkyBox::SkyBox(const Vec3& position) {
    this->initialize();
    this->setPosition(position);
}

SkyBox::~SkyBox() {
    glDeleteBuffers(1, &this->vertexBuffer);
}

void SkyBox::setPosition(const Vec3& position) {
    this->translationMatrix.set(0, 3, position.get(Vec3::X));
    this->translationMatrix.set(1, 3, position.get(Vec3::Y));
    this->translationMatrix.set(2, 3, position.get(Vec3::Z));
}

Vec3 SkyBox::getPosition() const {
    return Vec3(this->translationMatrix.get(0, 3),
                this->translationMatrix.get(1, 3),
                this->translationMatrix.get(2, 3));
}

void SkyBox::scale(float factor) {
    this->scalingMatrix.set(0, 0, factor);
    this->scalingMatrix.set(1, 1, factor);
    this->scalingMatrix.set(2, 2, factor);
}

void SkyBox::render() {
    if (this->diffuseTexture != NULL) {
        this->diffuseTexture->bind(RenderEffect::DIFFUSE_TEXTURE_UNIT);
    }

    if (this->effect != NULL) {
        this->effect->enable();
        this->effect->setLocalWorldMatrix(this->translationMatrix *
                                          this->scalingMatrix);
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(this->vertices) / sizeof(float) / 3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::initialize() {
    glGenBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
