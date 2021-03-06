#ifndef SPOTLIGHT_H
#define	SPOTLIGHT_H

#include "Light.h"
#include "Projection.h"
#include "PerspectiveProjection.h"

class SpotLight: public Light {
    public:
        SpotLight();
        ~SpotLight();
        
        const Projection* getShadowProjection() const {
            return this->shadowProjection;
        }
        
        LightData getLightData() const;
        
        float getSize() const {
            return this->size;
        }

        void setSize(float size) {
            this->shadowProjection->setFov(size);
            this->size = size;
            this->valid = false;
        }
        
        float getBlend() const {
            return this->blend;
        }

        void setBlend(float blend) {
            this->blend = blend;
            this->valid = false;
        }
        
        float getFalloff() const {
            return this->falloff;
        }

        void setFalloff(float falloff) {
            this->falloff = falloff;
            this->valid = false;
        }

    private:
        PerspectiveProjection* shadowProjection;
        
        float size;
        float blend;
        float falloff;
};

#endif	/* SPOTLIGHT_H */
