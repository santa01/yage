#ifndef ORTHOGRAPHICPROJECTION_H
#define	ORTHOGRAPHICPROJECTION_H

#include "Projection.h"

class OrthographicProjection: public Projection {
    public:
        OrthographicProjection();
        
        void setNearPlane(float nearPlane);
        float getNearPlane() const {
            return this->nearPlane;
        }
        
        void setFarPlane(float farPlane);
        float getFarPlane() const {
            return this->farPlane;
        }
        
        void setAspectRatio(float aspectRatio);
        float getAspectRatio() const {
            return this->aspectRatio;
        }
        
    private:
        void updatePerspectiveMatrix();
        
        float aspectRatio;
        float nearPlane;
        float farPlane;
};

#endif	/* ORTHOGRAPHICPROJECTION_H */
