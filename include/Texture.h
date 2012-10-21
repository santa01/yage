#ifndef TEXTURE_H
#define	TEXTURE_H

#include <global.h>

#include "Image.h"

class Texture {
    public:
        Texture();
        ~Texture();
        
        bool load(const Image& image);
        void bind(int target);

    private:
        Texture(const Texture&);
        Texture& operator =(const Texture&);

        GLuint texture;
        GLuint sampler;
};

#endif	/* TEXTURE_H */
