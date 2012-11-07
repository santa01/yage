#ifndef ROTATABLE_H
#define	ROTATABLE_H

#include "Vec3.h"

/**
 * \interface Rotatable
 * \brief An interface for giving rotating capabilities to objects
 * \image html roll_pitch_yaw.png
 */
class Rotatable {
    public:
        virtual ~Rotatable();
        
        /**
         * \fn roll
         * \brief Rotate around Z axis
         * \param angle float, angle in degrees
         */
        void roll(float angle) {
            this->rotate(Vec3(0.0f, 0.0f, 1.0f), angle);
        }

        /**
         * \fn yaw
         * \brief Rotate around Y axis
         * \param angle float, angle in degrees
         */
        void yaw(float angle) {
            this->rotate(Vec3(0.0f, 1.0f, 0.0f), angle);
        }

        /**
         * \fn pitch
         * \brief Rotate around X axis
         * \param angle float, angle in degrees
         */
        void pitch(float angle) {
            this->rotate(Vec3(1.0f, 0.0f, 0.0f), angle);
        }
        
        /**
         * \fn getXAngle
         * \brief Get current X angle of Rotatable object
         * \return float, current X angle in degrees
         */
        virtual float getXAngle() const = 0;
        /**
         * \fn getYAngle
         * \brief Get current Y angle of Rotatable object
         * \return float, current Y angle in degrees
         */
        virtual float getYAngle() const = 0;
        /**
         * \fn getZAngle
         * \brief Get current Z angle of Rotatable object
         * \return float, current Z angle in degrees
         */
        virtual float getZAngle() const = 0;

        /**
         * \fn rotate
         * \brief TODO
         * \param vector TODO
         * \param angle TODO
         */
        virtual void rotate(const Vec3& vector, float angle) = 0;
};

#endif	/* ROTATABLE_H */
