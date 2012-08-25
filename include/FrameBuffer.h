#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H

#include <GL/gl.h>

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        
        void attach();
        void bindShadowMap(int target);
  
    private:
        FrameBuffer(const FrameBuffer&);
        FrameBuffer& operator =(const FrameBuffer&);
        
        GLuint fbo;
        GLuint shadowMapTexture;
        GLuint shadowMapSampler;
};

#endif	/* FRAMEBUFFER_H */
