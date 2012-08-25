#ifndef LIGHT_H
#define	LIGHT_H

#include <string>

#include "Vec3.h"
#include "Mat4.h"

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
    char _directionPadding[4];  // 4 bytes
} LightData;                    // 64 bytes total

#pragma pack(pop)

class Light {
    public:
        static const int TYPE_DIRECTED = 0;
        static const int TYPE_POINT = 1;
        static const int TYPE_SPOT = 2;
        
        Light();
        virtual ~Light();
        
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
        
        virtual LightData getLightData() const = 0;
        
    protected:
        std::string name;
        Vec3 color;
        
        int type;
        float energy;
        bool shadow;
        bool valid;
};

#endif	/* LIGHT_H */
