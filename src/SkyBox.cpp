#include <global.h>

#include "SkyBox.h"
#include "Vec3.h"

// SkyBox vertices data is embedded into the source file,
// loading cube geometry from a .mesh file is an overkill.

// Define a 200x200x200 cube vertices (8 total). Cube size could
// be changed with scale() methods if nesessary.
const float SkyBox::vertices[] = {
     100.0f,  100.0f,  100.0f,  // 0
     100.0f,  100.0f, -100.0f,  // 1
     100.0f, -100.0f,  100.0f,  // 2
     100.0f, -100.0f, -100.0f,  // 3
    -100.0f,  100.0f,  100.0f,  // 4
    -100.0f,  100.0f, -100.0f,  // 5
    -100.0f, -100.0f,  100.0f,  // 6
    -100.0f, -100.0f, -100.0f,  // 7
};

// Define separate polygons (clockwise) as references to respective vertices [0..7].
// SkyBox::vertices and SkyBox::indices aren't supposed to be changed and
// define a constant `box' geometry for a SkyBox.
const int SkyBox::indices[] = {
    4, 0, 6,
    0, 2, 6,
    0, 1, 2,
    1, 3, 2,
    1, 5, 3,
    5, 7, 3,
    5, 4, 7,
    4, 6, 7,
    5, 1, 4,
    1, 0, 4,
    6, 2, 7,
    2, 3, 7,
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
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(2, this->buffers);
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

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, sizeof(this->indices) / sizeof(int), GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}

void SkyBox::initialize() {
    glGenBuffers(2, this->buffers);
    glGenVertexArrays(1, &this->vao);

    glBindVertexArray(this->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[SkyBox::VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[SkyBox::ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}
