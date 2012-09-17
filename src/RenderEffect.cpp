#include <memory.h>
#include <sstream>

#include "RenderEffect.h"
#include "ShaderLoader.h"
#include "Vec3.h"

RenderEffect::RenderEffect() {
    this->program = 0;
    
    this->modelViewPerspectiveMatrix = 0;
    this->localWorldMatrix = 0;
    
    this->cameraPosition = 0;
    this->diffuseTextureSampler = 0;
}

void RenderEffect::setModelViewPerspectiveMatrix(const Mat4& matrix) {
    this->enable();
    glUniformMatrix4fv(this->modelViewPerspectiveMatrix, 1, GL_TRUE, (GLfloat*)matrix.data());
}

void RenderEffect::setLocalWorldMatrix(const Mat4& matrix) {
    this->enable();
    glUniformMatrix4fv(this->localWorldMatrix, 1, GL_TRUE, (GLfloat*)matrix.data());
}

void RenderEffect::setLightModelViewPerspectiveMatrix(int lightIndex, const Mat4& matrix) {
    std::stringstream builder;
    builder << "lightModelViewPerspectiveMatrices[" << lightIndex << "]";

    this->enable();
    GLint lightModelViewPerspectiveMatrix = glGetUniformLocation(this->program, builder.str().c_str());
    glUniformMatrix4fv(lightModelViewPerspectiveMatrix, 1, GL_TRUE, (GLfloat*)matrix.data());
}

void RenderEffect::setCameraPosition(const Vec3& cameraPosition) {
    this->enable();
    glUniform3fv(this->cameraPosition, 1, cameraPosition.data());
}

void RenderEffect::setMaterial(GLuint materialBuffer) {
    this->enable();
    glBindBufferBase(GL_UNIFORM_BUFFER, RenderEffect::MATERIAL_PARAMETERS_BINDPOINT, materialBuffer);
}

void RenderEffect::setAmbientLight(GLuint ambientLightBuffer) {
    this->enable();
    glBindBufferBase(GL_UNIFORM_BUFFER, RenderEffect::AMBIENT_LIGHT_BINDPOINT, ambientLightBuffer);
}

void RenderEffect::setLightSources(GLuint lightSourcesBuffer) {
    this->enable();
    glBindBufferBase(GL_UNIFORM_BUFFER, RenderEffect::LIGHT_SOURCES_BINDPOINT, lightSourcesBuffer);
}

void RenderEffect::attachShader(const std::string& name) {
    if (this->program == 0) {
        this->shaderList.push_back(ShaderLoader::createShader(name));
    }
}

void RenderEffect::enable() {
    if (this->program == 0) {
        this->program = ShaderLoader::createProgram(this->shaderList);
        
        glUseProgram(this->program);

        this->modelViewPerspectiveMatrix = glGetUniformLocation(this->program, "modelViewPerspectiveMatrix");
        this->localWorldMatrix = glGetUniformLocation(this->program, "localWorldMatrix");

        this->cameraPosition = glGetUniformLocation(this->program, "cameraPosition");
        this->diffuseTextureSampler = glGetUniformLocation(this->program, "diffuseTextureSampler");
        glUniform1i(this->diffuseTextureSampler, RenderEffect::DIFFUSE_TEXTURE_UNIT);

        GLuint materialParameters = glGetUniformBlockIndex(this->program, "MaterialParameters");
        GLuint ambientLightIndex = glGetUniformBlockIndex(this->program, "AmbientLight");
        GLuint lightIndex = glGetUniformBlockIndex(this->program, "Light");
        
        glUniformBlockBinding(this->program, materialParameters, RenderEffect::MATERIAL_PARAMETERS_BINDPOINT);
        glUniformBlockBinding(this->program, ambientLightIndex, RenderEffect::AMBIENT_LIGHT_BINDPOINT);
        glUniformBlockBinding(this->program, lightIndex, RenderEffect::LIGHT_SOURCES_BINDPOINT);
    } else {
        glUseProgram(this->program);
    }
}
