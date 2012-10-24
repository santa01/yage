#ifndef CAMERA_H
#define	CAMERA_H

#include <cstdlib>

#include "Mat4.h"
#include "Vec3.h"
#include "Projection.h"
#include "Movable.h"
#include "Rotatable.h"

/**
 * \class Camera
 * \brief Represents a first-person camera
 */
class Camera: public Movable, public Rotatable {
    public:
        Camera();
        Camera(float x, float y, float z);
        Camera(const Vec3& position);
        ~Camera();

        using Movable::setPosition;
        
        void setPosition(const Vec3& position);
        Vec3 getPosition() const;
        
        const Mat4& getTranslationMatrix() const {
            return this->translationMatrix;
        }
        
        float getXAngle() const;
        float getYAngle() const;
        float getZAngle() const;
        
        void rotate(const Vec3& vector, float angle);
        
        const Mat4& getRotationMatrix() const {
            return this->rotationMatrix;
        }
        
        Projection* getProjection() {
            return this->projection;
        }

        void setProjection(const Projection* projection) {
            if (projection != NULL) {
                delete this->projection;
                this->projection = new Projection(*projection);
            }
        }
        
        Vec3 getUp() const;
        Vec3 getTarget() const;
        Vec3 getRight() const;
        
        void lookAt(float x, float y, float z);
        void lookAt(const Vec3& target);

    private:
        void updateRotationMatrix(const Vec3& right, const Vec3& up, const Vec3& target);
        void initialize();
        
        Projection* projection;
        Mat4 translationMatrix;
        Mat4 rotationMatrix;
};

#endif	/* CAMERA_H */
