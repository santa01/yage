#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H

#include <global.h>

#include "Texture.h"

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        
        void bind();
        
        void attachDepthTexture(const Texture& texture, int layer);
        void attachColorTexture(const Texture& texture, int layer);
  
    private:
        FrameBuffer(const FrameBuffer&);
        FrameBuffer& operator =(const FrameBuffer&);
        
        GLuint fbo;
};

#endif	/* FRAMEBUFFER_H */
