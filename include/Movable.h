#ifndef MOVABLE_H
#define	MOVABLE_H

#include "Vec3.h"

class Movable {
    public:
        virtual ~Movable();
        
        void setPosition(float x, float y, float z) {
            this->setPosition(Vec3(x, y, z));
        }
        
        virtual void setPosition(const Vec3& position) = 0;
        virtual Vec3 getPosition() const = 0;
};

#endif	/* MOVABLE_H */
