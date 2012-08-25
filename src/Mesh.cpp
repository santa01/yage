#include <memory.h>
#include <cmath>
#include <fstream>
#include <string>

#include "Mesh.h"
#include "Logger.h"
#include "Vec3.h"
#include "Quaternion.h"
#include "RenderEffect.h"

Mesh::Mesh() {
    this->initialize();
}

Mesh::Mesh(float x, float y, float z) {
    this->setPosition(x, y, z);
    this->initialize();
}

Mesh::Mesh(const Vec3& position) {
    this->setPosition(position);
    this->initialize();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(3, this->buffers);
    
    if (this->facesData) {
        delete[] this->facesData;
    }
    if (this->vertexData) {
        delete[] this->vertexData;
    }
}

bool Mesh::load(const std::string& name) {
    std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
    if (!file.good()) {
        Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open file %s", name.c_str());
        return false;
    }

    MeshHeader header;
    memset(&header, 0, sizeof(header));
    file.read((char*)&header, sizeof(header));
    if (memcmp((char*)header.magic, "MESH", sizeof(header.magic)))
        return false;
    
    if (this->facesData) {
        delete[] this->facesData;
    }
    if (this->vertexData) {
        delete[] this->vertexData;
    }
    
    // TODO read material parameters from file (update exporter as well)
    MaterialData data;
    memset(&data, 0, sizeof(data));
    
    data.diffuseIntensity = 0.5f;
    data.specularIntensity = 1.0f;
    data.specularExponent = 50.0f;
    
    int materialsCount = 1;
    
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Mesh::MATERIAL_BUFFER]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(materialsCount), &materialsCount);
    glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(data), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    int vertexDataLength = header.facesOffset - sizeof(MeshHeader);
    this->vertexData = new char[vertexDataLength];
    file.read(this->vertexData, vertexDataLength);
    
    int facesDataLength = header.textureOffset - header.facesOffset;
    this->facesData = new char[facesDataLength];
    file.read(this->facesData, facesDataLength);
    
    glBindVertexArray(this->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[Mesh::VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, vertexDataLength, this->vertexData, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);       // vertex coords
    glEnableVertexAttribArray(1);       // vertex normal
    glEnableVertexAttribArray(2);       // uv coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(header.vnOffset - sizeof(header)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(header.uvOffset - sizeof(header)));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[Mesh::ELEMENT_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesDataLength, this->facesData, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    return true;
}

void Mesh::setPosition(const Vec3& position) {
    this->translationMatrix.set(0, 3, position.get(Vec3::X));
    this->translationMatrix.set(1, 3, position.get(Vec3::Y));
    this->translationMatrix.set(2, 3, position.get(Vec3::Z));
}

Vec3 Mesh::getPosition() const {
    return Vec3(this->translationMatrix.get(0, 3),
                this->translationMatrix.get(1, 3),
                this->translationMatrix.get(2, 3));
}

float Mesh::getXAngle() const {
    // TODO: implement
    return 0.0f;
}

float Mesh::getYAngle() const {
    // TODO: implement
    return 0.0f;
}

float Mesh::getZAngle() const {
    // TODO: implement
    return 0.0f;
}

void Mesh::rotate(const Vec3& vector, float angle) {
    Quaternion q(vector, angle * M_PI / 180.0f);
    q.normalize();
    
    this->rotationMatrix = q.extractMat4();
}

void Mesh::scaleX(float factor) {
    this->scalingMatrix.set(0, 0, factor);
}

void Mesh::scaleY(float factor) {
    this->scalingMatrix.set(1, 1, factor);
}

void Mesh::scaleZ(float factor) {
    this->scalingMatrix.set(2, 2, factor);
}

float Mesh::getXFactor() const {
    return this->scalingMatrix.get(0, 0);
}

float Mesh::getYFactor() const {
    return this->scalingMatrix.get(1, 1);
}

float Mesh::getZFactor() const {
    return this->scalingMatrix.get(2, 2);
}

void Mesh::scale(float factor) {
    this->scalingMatrix.set(0, 0, factor);
    this->scalingMatrix.set(1, 1, factor);
    this->scalingMatrix.set(2, 2, factor);
}

void Mesh::render() {
    if (this->diffuseTexture != NULL) {
        this->diffuseTexture->bind(RenderEffect::DIFFUSE_TEXTURE_UNIT);
    }

    if (this->effect != NULL) {
        this->effect->enable();
        this->effect->setMaterial(this->buffers[Mesh::MATERIAL_BUFFER]);
        this->effect->setMVMatrix(this->translationMatrix *
                                  this->rotationMatrix *
                                  this->scalingMatrix);
    }

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)0);
    glBindVertexArray(0);
}

void Mesh::initialize() {
    glGenBuffers(3, this->buffers);
    glGenVertexArrays(1, &this->vao);

    MaterialData data;
    memset(&data, 0, sizeof(data));

    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Mesh::MATERIAL_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, 16 + sizeof(data) * Mesh::MAX_MATERIALS, &data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    this->facesData = NULL;
    this->vertexData = NULL;
}
