#include <memory.h>
#include <sstream>

#include "RenderEffect.h"
#include "ShaderLoader.h"
#include "Vec3.h"

RenderEffect::RenderEffect() {
    this->program = 0;
    
    this->modelViewPerspectiveMatrix = -1;
    this->localWorldMatrix = -1;
    this->cameraPosition = -1;
    
    memset(this->lightModelViewPerspectiveMatrices, -1, sizeof(this->lightModelViewPerspectiveMatrices));
    memset(this->shadowMapSamplers, -1, sizeof(this->shadowMapSamplers));
}

void RenderEffect::setModelViewPerspectiveMatrix(const Mat4& matrix) {
    this->enable();
    
    if (this->modelViewPerspectiveMatrix > -1) {
        glUniformMatrix4fv(this->modelViewPerspectiveMatrix, 1, GL_TRUE, (GLfloat*)matrix.data());
    }
}

void RenderEffect::setLocalWorldMatrix(const Mat4& matrix) {
    this->enable();
    
    if (this->localWorldMatrix > -1) {
        glUniformMatrix4fv(this->localWorldMatrix, 1, GL_TRUE, (GLfloat*)matrix.data());
    }
}

void RenderEffect::setLightModelViewPerspectiveMatrix(int lightIndex, const Mat4& matrix) {
    this->enable();
    
    if (this->lightModelViewPerspectiveMatrices[lightIndex] > -1) {
        glUniformMatrix4fv(this->lightModelViewPerspectiveMatrices[lightIndex], 1, GL_TRUE, (GLfloat*)matrix.data());
    }
}

void RenderEffect::setShadowMapSampler(int lightIndex, int textureUnit) {
    this->enable();

    if (this->shadowMapSamplers[lightIndex] > -1) {
        glUniform1i(this->shadowMapSamplers[lightIndex], textureUnit);
    }
}

void RenderEffect::setCameraPosition(const Vec3& cameraPosition) {
    this->enable();
    
    if (this->cameraPosition > -1) {
        glUniform3fv(this->cameraPosition, 1, cameraPosition.data());
    }
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

        for (int i = 0; i < RenderEffect::MAX_LIGHTS; i++) {
            std::stringstream builder;
            builder << "lightModelViewPerspectiveMatrices[" << i << "]";

            this->lightModelViewPerspectiveMatrices[i] = glGetUniformLocation(this->program, builder.str().c_str());
            if (this->lightModelViewPerspectiveMatrices[i] == -1) {
                break;  // Looks like there is no such uniform array
            }
        }
        
        for (int i = 0; i < RenderEffect::MAX_LIGHTS; i++) {
            std::stringstream builder;
            builder << "shadowMapSamplers[" << i << "]";

            this->shadowMapSamplers[i] = glGetUniformLocation(this->program, builder.str().c_str());
            if (this->shadowMapSamplers[i] == -1) {
                break;
            }
        }
        
        GLint diffuseTextureSampler = glGetUniformLocation(this->program, "diffuseTextureSampler");
        if (diffuseTextureSampler > -1) {
            glUniform1i(diffuseTextureSampler, RenderEffect::DIFFUSE_TEXTURE_UNIT);
        }

        GLuint materialParameters = glGetUniformBlockIndex(this->program, "MaterialParameters");
        if (materialParameters != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, materialParameters, RenderEffect::MATERIAL_PARAMETERS_BINDPOINT);
        }
        
        GLuint ambientLightIndex = glGetUniformBlockIndex(this->program, "AmbientLight");
        if (ambientLightIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, ambientLightIndex, RenderEffect::AMBIENT_LIGHT_BINDPOINT);
        }
        
        GLuint lightIndex = glGetUniformBlockIndex(this->program, "Light");
        if (lightIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, lightIndex, RenderEffect::LIGHT_SOURCES_BINDPOINT);   
        }
    } else {
        glUseProgram(this->program);
    }
}
