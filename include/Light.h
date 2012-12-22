#ifndef LIGHT_H
#define	LIGHT_H

#include <string>

#include "Vec3.h"
#include "Mat4.h"
#include "Projection.h"
#include "Rotatable.h"
#include "Movable.h"

#pragma pack(push, 1)

// Padded to be used as an array element in std140 layout uniform blocks
typedef struct {
    int type;                   // 4 bytes (ooh...)
    float energy;               // 4 bytes
    float falloff;              // 4 bytes
    float size;                 // 4 bytes
    float color[3];             // 12 bytes, aligned to vec4 size (16 bytes)
    float blend;                // 4 bytes
    float position[3];          // 12 bytes, aligned to vec4 size (16 bytes)
    int shadow;                 // 4 bytes
    float direction[3];         // 12 bytes, aligned to vec4 size (16 bytes)
    int _padding;               // 4 bytes
} LightData;                    // 64 bytes total

#pragma pack(pop)

class Light: public Movable, public Rotatable {
    public:
        enum {
            TYPE_DIRECTED,
            TYPE_POINT,
            TYPE_SPOT
        };
        
        Light();
        virtual ~Light();

        using Movable::setPosition;
        
        void setPosition(float x, float y, float z) {
            this->setPosition(Vec3(x, y, z));
        }
        
        void setPosition(const Vec3& position) {
            this->position = position;
            this->valid = false;
        }
        
        Vec3 getPosition() const {
            return this->position;
        }

        float getXAngle() const;
        float getYAngle() const;
        float getZAngle() const;

        void rotate(const Vec3& vector, float angle);
        
        void setDirection(float x, float y, float z) {
            this->setDirection(Vec3(x, y, z));
        }
        
        void setDirection(const Vec3& direction) {
            this->direction = direction;
            this->valid = false;
        }
        
        const Vec3& getDirection() const {
            return this->direction;
        }
        
        int getType() const {
            return this->type;
        }

        void setName(const std::string& name) {
            this->name = name;
        }
        
        const std::string& getName() const {
            return this->name;
        }
        
        void setColor(const Vec3& color) {
            this->color = color;
            this->valid = false;
        }
        
        const Vec3& getColor() const {
            return this->color;
        }

        void setEnergy(float energy) {
            this->energy = energy;
            this->valid = false;
        }
        
        float getEnergy() const {
            return this->energy;
        }
        
        void setCastsShadow(bool castsShadow) {
            this->shadowCaster = castsShadow;
            this->valid = false;
        }
        
        bool castsShadow() const {
            return this->shadowCaster;
        }

        virtual const Projection* getShadowProjection() const = 0;
        virtual LightData getLightData() const = 0;
        
    protected:
        std::string name;
        Vec3 color;
        Vec3 direction;
        Vec3 position;
        
        int type;
        float energy;
        bool shadowCaster;
        
        friend class Scene;
        bool valid;     // Should be set to false whenever light properties are changed.
        bool raw;       // Sholud be initially true, indicates that this light is new.
};

#endif	/* LIGHT_H */
