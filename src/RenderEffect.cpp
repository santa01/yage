#include <memory.h>

#include "RenderEffect.h"
#include "ShaderLoader.h"
#include "Vec3.h"

RenderEffect::RenderEffect() {
    this->program = 0;
    
    this->mvp = 0;
    this->mv = 0;
    this->cameraPosition = 0;
    this->textureSampler = 0;
}

void RenderEffect::setMVPMatrix(const Mat4& mvpMatrix) {
    this->enable();
    glUniformMatrix4fv(this->mvp, 1, GL_TRUE, (GLfloat*)mvpMatrix.data());
}

void RenderEffect::setMVMatrix(const Mat4& mvMatrix) {
    this->enable();
    glUniformMatrix4fv(this->mv, 1, GL_TRUE, (GLfloat*)mvMatrix.data());
}

void RenderEffect::setLightMVPMatrix(const Mat4& lightSpaceMvpMatrix) {
    this->enable();
    glUniformMatrix4fv(this->lightSpaceMvp, 1, GL_TRUE, (GLfloat*)lightSpaceMvpMatrix.data());
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

        this->mvp = glGetUniformLocation(this->program, "mvp");
        this->mv = glGetUniformLocation(this->program, "mv");
        this->lightSpaceMvp = glGetUniformLocation(this->program, "lightSpaceMvp");
        this->cameraPosition = glGetUniformLocation(this->program, "cameraPosition");
        
        GLint shadowSampler = glGetUniformLocation(this->program, "shadowMapSamplers[0]");
        glUniform1i(shadowSampler, 2);

        this->textureSampler = glGetUniformLocation(this->program, "textureSampler");
        glUniform1i(this->textureSampler, RenderEffect::DIFFUSE_TEXTURE_UNIT);

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
