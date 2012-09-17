#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include "Light.h"
#include "Vec3.h"
#include "Projection.h"
#include "PerspectiveProjection.h"

class PointLight: public Light {
    public:
        PointLight();
        ~PointLight();
        
        Projection* getShadowProjection() const {
            return this->shadowProjection;
        }

        LightData getLightData() const;
        
        float getFalloff() const {
            return this->falloff;
        }

        void setFalloff(float falloff) {
            this->falloff = falloff;
            this->setValid(false);
        }

    private:
        float falloff;
        PerspectiveProjection* shadowProjection;
};

#endif	/* POINTLIGHT_H */
