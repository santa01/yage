#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include "Light.h"
#include "Projection.h"
#include "PerspectiveProjection.h"

class PointLight: public Light {
    public:
        PointLight();
        ~PointLight();

        const Projection* getShadowProjection() const {
            return this->shadowProjection;
        }

        LightData getLightData() const;
        
        float getFalloff() const {
            return this->falloff;
        }

        void setFalloff(float falloff) {
            this->falloff = falloff;
            this->valid = false;
        }

    private:
        PerspectiveProjection* shadowProjection;
        
        float falloff;
};

#endif	/* POINTLIGHT_H */
