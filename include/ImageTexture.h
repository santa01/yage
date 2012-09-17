#ifndef IMAGETEXTURE_H
#define	IMAGETEXTURE_H

#include "Image.h"
#include "Texture.h"

class ImageTexture: public Texture {
    public:
        ImageTexture();
        
        void setAnisotropyLevel(int anisotropyLevel);
        int getAnisotropyLevel() const {
            return this->anisotropyLevel;
        }

        bool load(const Image& image);
        
    private:
        int anisotropyLevel;
};

#endif	/* IMAGETEXTURE_H */
