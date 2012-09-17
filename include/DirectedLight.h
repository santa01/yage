#ifndef DIRECTEDLIGHT_H
#define	DIRECTEDLIGHT_H

#include "Light.h"
#include "Vec3.h"
#include "Projection.h"
#include "OrthographicProjection.h"

class DirectedLight: public Light {
    public:
        DirectedLight();
        ~DirectedLight();
        
        Projection* getShadowProjection() const {
            return this->shadowProjection;
        }

        LightData getLightData() const;

    private:
        OrthographicProjection* shadowProjection;
};

#endif	/* DIRECTEDLIGHT_H */
