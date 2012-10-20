#ifndef SHADOWMAP_H
#define	SHADOWMAP_H

#include "Texture.h"
#include "Vec3.h"

class ShadowMap: public Texture {
    public:
        ShadowMap();
        ShadowMap(int dimention);
        
        void setDimention(int dimention);
        
    private:
        void initialize();
};

#endif	/* SHADOWMAP_H */
