#ifndef IMAGETEXTURE_H
#define	IMAGETEXTURE_H

#include "Image.h"
#include "Texture.h"

class ImageTexture: public Texture {
    public:
        ImageTexture();
        ImageTexture(float anisotropyLevel);

        void setAnisotropyLevel(float anisotropyLevel);
        float getAnisotropyLevel() const {
            return this->anisotropyLevel;
        }

        bool load(const Image& image);
        
    private:
        void initialize();
        
        float anisotropyLevel;
};

#endif	/* IMAGETEXTURE_H */
