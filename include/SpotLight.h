#ifndef SPOTLIGHT_H
#define	SPOTLIGHT_H

#include "Light.h"
#include "Rotatable.h"
#include "Movable.h"

class SpotLight: public Light, public Rotatable, public Movable {
    public:
        SpotLight();
        ~SpotLight();

        using Movable::setPosition;
        
        void setPosition(const Vec3& position) {
            this->position = position;
        }
        
        Vec3 getPosition() const {
            return this->position;
        }
        
        float getXAngle() const;
        float getYAngle() const;
        float getZAngle() const;

        void rotate(const Vec3& vector, float angle);
        
        LightData getLightData() const;
        
        void setDirection(float x, float y, float z) {
            this->setDirection(Vec3(x, y, z));
        }
        
        void setDirection(const Vec3& direction) {
            this->direction = direction;
        }
        
        const Vec3& getDirection() const {
            return this->direction;
        }
        
        float getSize() const {
            return this->size;
        }

        void setSize(float size) {
            this->size = size;
        }
        
        float getBlend() const {
            return this->blend;
        }

        void setBlend(float blend) {
            this->blend = blend;
        }
        
        float getFalloff() const {
            return this->falloff;
        }

        void setFalloff(float falloff) {
            this->falloff = falloff;
        }

    private:
        Vec3 direction;
        Vec3 position;
        
        float size;
        float blend;
        float falloff;
};

#endif	/* SPOTLIGHT_H */
