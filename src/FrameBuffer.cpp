#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenSamplers(1, &this->shadowMapSampler);
    glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(this->shadowMapSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glGenTextures(1, &this->shadowMapTexture);
    glBindTexture(GL_TEXTURE_2D, this->shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, this->shadowMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_DRAW_BUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &this->fbo);
    glDeleteTextures(1, &this->shadowMapTexture);
    glDeleteSamplers(1, &this->shadowMapSampler);
}

void FrameBuffer::attach() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
}

void FrameBuffer::bindShadowMap(int target) {
    glActiveTexture(GL_TEXTURE0 + target);
    glBindTexture(GL_TEXTURE_2D, this->shadowMapTexture);
    glBindSampler(target, this->shadowMapSampler);
}
