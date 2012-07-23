#ifndef DIRECTEDLIGHT_H
#define	DIRECTEDLIGHT_H

#include "Light.h"
#include "Rotatable.h"
#include "Vec3.h"

class DirectedLight: public Light, public Rotatable {
    public:
        DirectedLight();
        ~DirectedLight();
        
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

    private:
        Vec3 direction;
};

#endif	/* DIRECTEDLIGHT_H */
