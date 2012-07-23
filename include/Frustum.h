#ifndef FRUSTUM_H
#define	FRUSTUM_H

#include "Mat4.h"

class Frustum {
    public:
        Frustum();
        virtual ~Frustum();
        
        void setNearPlane(float nearPlane);
        float getNearPlane() const {
            return this->nearPlane;
        }
        
        void setFarPlane(float farPlane);
        float getFarPlane() const {
            return this->farPlane;
        }

        void setFov(float fov);
        float getFov() const {
            return this->fov;
        }
        
        void setAspectRatio(float aspectRatio);
        float getAspectRatio() const {
            return this->aspectRatio;
        }
        
        const Mat4& getPerspectiveMatrix() const {
            return this->perspective;
        }

    protected:
        Mat4 perspective;
        float aspectRatio;
        float nearPlane;
        float farPlane;
        float fov;
        
        void updateClipDistances();
        void updateFieldOfView();
};

#endif	/* FRUSTUM_H */
