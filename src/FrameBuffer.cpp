#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &this->fbo);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &this->fbo);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
}

void FrameBuffer::attachDepthTexture(const Texture& texture, int layer) {
    glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            texture.getTextureHandle(), 0, layer);
}

void FrameBuffer::attachColorTexture(const Texture& texture, int layer) {
    glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            texture.getTextureHandle(), 0, layer);
}
