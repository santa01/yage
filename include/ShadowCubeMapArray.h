#ifndef SHADOWCUBEMAPARRAY_H
#define	SHADOWCUBEMAPARRAY_H

#include "Texture.h"
#include "Vec3.h"

class ShadowCubeMapArray: public Texture {
    public:
        static const Vec3 getFaceDirection(int i);
        
        ShadowCubeMapArray();
        ShadowCubeMapArray(int dimension);
        
        void setDimension(int dimension);
        
    private:
        void initialize();
};

#endif	/* SHADOWCUBEMAPARRAY_H */
