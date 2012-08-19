#ifndef SCALABLE_H
#define	SCALABLE_H

/**
 * \interface Scalable
 * \brief An interface for giving scaling capabilities to objects
 */
class Scalable {
    public:
        virtual ~Scalable();

        /**
         * \fn scaleX
         * \brief Change Scalable size against X axis
         * \param factor float, a relative size for X scale
         */
        virtual void scaleX(float factor) = 0;
        /**
         * \fn scaleY
         * \brief Change Scalable size against Y axis
         * \param factor float, a relative size for Y scale
         */
        virtual void scaleY(float factor) = 0;
        /**
         * \fn scaleZ
         * \brief Change Scalable size against Z axis
         * \param factor float, a relative size for Z scale
         */
        virtual void scaleZ(float factor) = 0;
        
        /**
         * \fn getXFactor
         * \brief Get current X scale factor
         * \return float, X scale factor
         */
        virtual float getXFactor() const = 0;
        /**
         * \fn getYFactor
         * \brief Get current Y scale factor
         * \return float, X scale factor
         */
        virtual float getYFactor() const = 0;
        /**
         * \fn getZFactor
         * \brief Get current Z scale factor
         * \return float, Z scale factor
         */
        virtual float getZFactor() const = 0;
        
        /**
         * \fn scale
         * \brief Change Scalable size against all axes (X, Y, Z)
         * \param factor float, a relative size for X, Y and Z scale
         */
        virtual void scale(float factor) = 0;
};

#endif	/* SCALABLE_H */
