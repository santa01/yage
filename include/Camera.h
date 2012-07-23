#ifndef CAMERA_H
#define	CAMERA_H

#include "Mat4.h"
#include "Vec3.h"
#include "Frustum.h"
#include "Movable.h"
#include "Rotatable.h"

class Camera: public Frustum, public Movable, public Rotatable {
    public:
        Camera();
        Camera(float x, float y, float z);
        Camera(const Vec3& position);

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
        
        Vec3 getUpVector() const {
            return this->up;
        }
        
        Vec3 getTargetVector() const {
            return this->target;
        }
        
        Vec3 getRightVector() const;
        
        void lookAt(float x, float y, float z);
        void lookAt(const Vec3& target);

    private:
        Vec3 up;
        Vec3 target;
        Mat4 translationMatrix;
        Mat4 rotationMatrix;
};

#endif	/* CAMERA_H */
