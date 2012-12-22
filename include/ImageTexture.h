#ifndef IMAGETEXTURE_H
#define	IMAGETEXTURE_H

#include "Image.h"
#include "Texture.h"

class ImageTexture: public Texture {
    public:
        ImageTexture();
        ImageTexture(int anisotropyLevel);

        int getAnisotropyLevel() const {
            return this->anisotropyLevel;
        }

        bool load(const Image& image);
        
    private:
        void initialize();
        
        int anisotropyLevel;
};

#endif	/* IMAGETEXTURE_H */
