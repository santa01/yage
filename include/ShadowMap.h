#ifndef SHADOWMAP_H
#define	SHADOWMAP_H

#include "Texture.h"

class ShadowMap: public Texture {
    public:
        ShadowMap();
        
        void setDimention(int dimention);
        int getDimention() const {
            return this->dimention;
        }
        
    private:
        int dimention;
};

#endif	/* SHADOWMAP_H */
