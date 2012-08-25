#ifndef POINTLIGHT_H
#define	POINTLIGHT_H

#include "Light.h"
#include "Movable.h"
#include "Vec3.h"

class PointLight: public Light, public Movable {
    public:
        PointLight();
        ~PointLight();
        
        using Movable::setPosition;

        void setPosition(float x, float y, float z) {
            this->setPosition(Vec3(x, y, z));
        }
        
        void setPosition(const Vec3& position) {
            this->position = position;
            this->setValid(false);
        }
        
        Vec3 getPosition() const {
            return this->position;
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
        Vec3 position;        
        float falloff;
};

#endif	/* POINTLIGHT_H */
