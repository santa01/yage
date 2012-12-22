#ifndef PROJECTION_H
#define	PROJECTION_H

#include "Mat4.h"

class Projection {
    public:
        virtual ~Projection();
        
        const Mat4& getProjectionMatrix() const {
            return this->projection;
        }
        
    protected:
        Mat4 projection;
};

#endif	/* PROJECTION_H */
