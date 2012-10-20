#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include "Light.h"
#include "Vec3.h"
#include "Projection.h"
#include "PerspectiveProjection.h"
#include "ShadowMap.h"
#include "Texture.h"

class PointLight: public Light {
    public:
        PointLight();
        ~PointLight();
        
        Projection* getShadowProjection() const {
            return this->shadowProjection;
        }

        LightData getLightData() const;
        
        Texture& getShadowMap() {
            return this->shadowMap;
        }
        
        float getFalloff() const {
            return this->falloff;
        }

        void setFalloff(float falloff) {
            this->falloff = falloff;
            this->setValid(false);
        }

    private:
        PerspectiveProjection* shadowProjection;
        ShadowMap shadowMap;
        
        float falloff;
};

#endif	/* POINTLIGHT_H */
