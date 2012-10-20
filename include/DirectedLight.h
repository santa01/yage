#ifndef DIRECTEDLIGHT_H
#define	DIRECTEDLIGHT_H

#include "Light.h"
#include "Vec3.h"
#include "Projection.h"
#include "OrthographicProjection.h"
#include "ShadowMap.h"
#include "Texture.h"

class DirectedLight: public Light {
    public:
        DirectedLight();
        ~DirectedLight();
        
        Projection* getShadowProjection() const {
            return this->shadowProjection;
        }

        LightData getLightData() const;
        
        Texture& getShadowMap() {
            return this->shadowMap;
        }
        
    private:
        OrthographicProjection* shadowProjection;
        ShadowMap shadowMap;
};

#endif	/* DIRECTEDLIGHT_H */
