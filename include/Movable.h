#ifndef MOVABLE_H
#define	MOVABLE_H

#include "Vec3.h"

/**
 * \interface Movable
 * \brief An interface for giving moving capabilities to objects
 * Every scene object that can be moved in game world must implement this interface
 */
class Movable {
    public:
        virtual ~Movable();
        
        /**
         * \fn setPosition(float x, float y, float z)
         * \brief Set the Movable object's position
         * \param x float, the X component of position vector
         * \param y float, the Y component of position vector
         * \param z float, the Y component of position vector
         */
        void setPosition(float x, float y, float z) {
            this->setPosition(Vec3(x, y, z));
        }
        
        /**
         * \fn setPosition(const Vec3& position)
         * \brief Set movable object's position
         * \param position Vec3, a vector with new coordinates
         */
        virtual void setPosition(const Vec3& position) = 0;
        /**
         * \fn getPosition
         * \brief Get position of Movable object
         * \return Vec3, current object's position
         */
        virtual Vec3 getPosition() const = 0;
};

#endif	/* MOVABLE_H */
