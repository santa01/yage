#ifndef ROTATABLE_H
#define	ROTATABLE_H

#include "Vec3.h"

class Rotatable {
    public:
        virtual ~Rotatable();
        
        void roll(float angle) {
            this->rotate(Vec3(0.0f, 0.0f, 1.0f), angle);
        }
        
        void yaw(float angle) {
            this->rotate(Vec3(0.0f, 1.0f, 0.0f), angle);
        }
        
        void pitch(float angle) {
            this->rotate(Vec3(1.0f, 0.0f, 0.0f), angle);
        }
        
        virtual float getXAngle() const = 0;
        virtual float getYAngle() const = 0;
        virtual float getZAngle() const = 0;

        virtual void rotate(const Vec3& vector, float angle) = 0;
};

#endif	/* ROTATABLE_H */
