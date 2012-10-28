#ifndef SHADOWMAPARRAY_H
#define	SHADOWMAPARRAY_H

#include "Texture.h"
#include "Vec3.h"

class ShadowMapArray: public Texture {
    public:
        ShadowMapArray();
        ShadowMapArray(int dimension);

        void setDimention(int dimention);
        int getDimention() const {
            return this->width;
        }

    private:
        void initialize();
};

#endif	/* SHADOWMAPARRAY_H */
