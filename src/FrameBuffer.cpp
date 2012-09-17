#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_DRAW_BUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &this->fbo);
}

void FrameBuffer::attach() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
}

void FrameBuffer::bindTexture(const Texture& texture) {
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, texture.getTextureHandle(), 0);
}
