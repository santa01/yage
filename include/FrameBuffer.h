#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H

#include <GL/gl.h>

#include "Texture.h"

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        
        void bind();
        void attachTexture(const Texture& texture);
  
    private:
        FrameBuffer(const FrameBuffer&);
        FrameBuffer& operator =(const FrameBuffer&);
        
        GLuint fbo;
};

#endif	/* FRAMEBUFFER_H */
