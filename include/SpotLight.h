#ifndef SPOTLIGHT_H
#define	SPOTLIGHT_H

#include "Light.h"
#include "Projection.h"
#include "PerspectiveProjection.h"
#include "ShadowMap.h"
#include "Texture.h"

class SpotLight: public Light {
    public:
        SpotLight();
        ~SpotLight();
        
        Projection* getShadowProjection() const {
            return this->shadowProjection;
        }
        
        LightData getLightData() const;
        
        Texture& getShadowMap() {
            return this->shadowMap;
        }
        
        float getSize() const {
            return this->size;
        }

        void setSize(float size) {
            this->size = size;
            this->shadowProjection->setFov(size);
            this->setValid(false);
        }
        
        float getBlend() const {
            return this->blend;
        }

        void setBlend(float blend) {
            this->blend = blend;
            this->setValid(false);
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
        
        float size;
        float blend;
        float falloff;
};

#endif	/* SPOTLIGHT_H */
