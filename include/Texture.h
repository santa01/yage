#ifndef TEXTURE_H
#define	TEXTURE_H

#include <global.h>

class Texture {
    public:
        Texture();
        virtual ~Texture();
        
        GLuint getTextureHandle() const {
            return this->texture;
        }
        
        void bind(int target);

    protected:
        GLuint texture;
        
    private:
        Texture(const Texture&);
        Texture& operator =(const Texture&);
};

#endif	/* TEXTURE_H */
