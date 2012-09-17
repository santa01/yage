#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H

#include <GL/gl.h>

#include "Texture.h"

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        
        void attach();
        void bindTexture(const Texture& texture);
  
    private:
        FrameBuffer(const FrameBuffer&);
        FrameBuffer& operator =(const FrameBuffer&);
        
        GLuint fbo;
};

#endif	/* FRAMEBUFFER_H */
