#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#include <global.h>

#include "Image.h"
#include "Texture.h"

class CubeTexture: public Texture {
    public:
        // This is a direct mapping between GL_TEXTURE_CUBE_MAP
        // constants and their aliases.
        enum {
            RIGHT_FACE = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            LEFT_FACE = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            BOTTOM_FACE = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            TOP_FACE = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            FRONT_FACE = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            BACK_FACE = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };

        CubeTexture();
        CubeTexture(float anisotropyLevel);

        void setAnisotropyLevel(float anisotropyLevel);
        float getAnisotropyLevel() const {
            return this->anisotropyLevel;
        }

        bool loadFaceImage(int face, const Image& image);

    private:
        void initialize();

        float anisotropyLevel;
};

#endif /* CUBETEXTURE_H */
