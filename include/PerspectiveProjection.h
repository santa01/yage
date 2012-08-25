#ifndef PERSPECTIVEPROJECTION_H
#define	PERSPECTIVEPROJECTION_H

#include "Projection.h"

class PerspectiveProjection: public Projection {
    public:
        PerspectiveProjection();
        
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

    protected:        
        float aspectRatio;
        float nearPlane;
        float farPlane;
        float fov;
        
        void updateClipDistances();
        void updateFieldOfView();
};

#endif	/* PERSPECTIVEPROJECTION_H */
