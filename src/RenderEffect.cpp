#include <sstream>

#include "RenderEffect.h"
#include "ShaderLoader.h"
#include "Vec3.h"

RenderEffect::RenderEffect() {
    this->program = 0;
}

RenderEffect::~RenderEffect() {
    if (this->program != 0) {
        glDeleteProgram(this->program);
    }
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
        this->shaderList.push_back(name);
    }
}

void RenderEffect::enable() {
    if (this->program == 0) {
        this->program = ShaderLoader::createProgram(this->shaderList);
        glUseProgram(this->program);

        this->modelViewPerspectiveMatrix = glGetUniformLocation(this->program, "modelViewPerspectiveMatrix");
        this->localWorldMatrix = glGetUniformLocation(this->program, "localWorldMatrix");
        this->cameraPosition = glGetUniformLocation(this->program, "cameraPosition");

        for (unsigned int i = 0; i < RenderEffect::MAX_LIGHTS; i++) {
            std::stringstream builder;
            builder << "lightModelViewPerspectiveMatrices[" << i << "]";

            this->lightModelViewPerspectiveMatrices[i] = glGetUniformLocation(this->program, builder.str().c_str());
            if (this->lightModelViewPerspectiveMatrices[i] == -1) {
                break;  // Looks like there is no such uniform array
            }
        }
        
        GLint samplerLocation = glGetUniformLocation(this->program, "diffuseTextureSampler");
        if (samplerLocation > -1) {
            glUniform1i(samplerLocation, RenderEffect::DIFFUSE_TEXTURE_UNIT);
        }
        
        samplerLocation = glGetUniformLocation(this->program, "specularTextureSampler");
        if (samplerLocation > -1) {
            glUniform1i(samplerLocation, RenderEffect::SPECULAR_TEXTURE_UNIT);
        }
        
        samplerLocation = glGetUniformLocation(this->program, "shadowMapArraySampler");
        if (samplerLocation > -1) {
            glUniform1i(samplerLocation, RenderEffect::SHADOW_MAP_ARRAY_TEXTURE_UNIT);
        }

        samplerLocation = glGetUniformLocation(this->program, "shadowCubeMapArraySampler");
        if (samplerLocation > -1) {
            glUniform1i(samplerLocation, RenderEffect::SHADOW_CUBE_MAP_ARRAY_TEXTURE_UNIT);
        }

        GLuint uniformBlockIndex = glGetUniformBlockIndex(this->program, "MaterialParameters");
        if (uniformBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, uniformBlockIndex, RenderEffect::MATERIAL_PARAMETERS_BINDPOINT);
        }
        
        uniformBlockIndex = glGetUniformBlockIndex(this->program, "AmbientLight");
        if (uniformBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, uniformBlockIndex, RenderEffect::AMBIENT_LIGHT_BINDPOINT);
        }
        
        uniformBlockIndex = glGetUniformBlockIndex(this->program, "Light");
        if (uniformBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, uniformBlockIndex, RenderEffect::LIGHT_SOURCES_BINDPOINT);   
        }
    } else {
        glUseProgram(this->program);
    }
}
