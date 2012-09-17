#ifndef LIGHT_H
#define	LIGHT_H

#include <string>

#include "Vec3.h"
#include "Mat4.h"
#include "Projection.h"
#include "ShadowMap.h"
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
        static const int TYPE_DIRECTED = 0;
        static const int TYPE_POINT = 1;
        static const int TYPE_SPOT = 2;
        
        Light();
        virtual ~Light();

        using Movable::setPosition;
        
        void setPosition(float x, float y, float z) {
            this->setPosition(Vec3(x, y, z));
        }
        
        void setPosition(const Vec3& position) {
            this->position = position;
            this->setValid(false);
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
            this->setValid(false);
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
            this->setValid(false);
        }
        
        const Vec3& getColor() const {
            return this->color;
        }
        
        ShadowMap& getShadowMap() {
            return this->shadowMap;
        }
        
        void setEnergy(float energy) {
            this->energy = energy;
            this->setValid(false);
        }
        
        float getEnergy() const {
            return this->energy;
        }
        
        void setShadow(bool shadow) {
            this->shadow = shadow;
            this->setValid(false);
        }
        
        bool isShadow() const {
            return this->shadow;
        }
        
        void setValid(bool valid) {
            this->valid = valid;
        }
        
        bool isValid() const {
            return this->valid;
        }

        void setRaw(bool raw) {
            this->raw = raw;
        }

        bool isRaw() const {
            return this->raw;
        }

        virtual Projection* getShadowProjection() const = 0;
        virtual LightData getLightData() const = 0;
        
    protected:
        std::string name;
        Vec3 color;
        Vec3 direction;
        Vec3 position;
        ShadowMap shadowMap;
        
        int type;
        float energy;
        bool shadow;
        bool valid;
        bool raw;
};

#endif	/* LIGHT_H */
