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
        
        GLenum getTextureType() const {
            return this->type;
        }

        int getWidth() const {
            return this->width;
        }
        
        int getHeight() const {
            return this->height;
        }

        void bind(int target);

    protected:
        GLuint texture;
        GLenum type;

        int width;
        int height;
        
    private:
        Texture(const Texture&);
        Texture& operator =(const Texture&);
};

#endif	/* TEXTURE_H */
