#ifndef MESH_H
#define	MESH_H

#include <global.h>
#include <stdint.h>
#include <string>

#include "Vec3.h"
#include "Mat4.h"
#include "Movable.h"
#include "Renderable.h"
#include "Scalable.h"
#include "Rotatable.h"

#pragma pack(push, 1)

typedef struct {
    int8_t magic[4];          // Should be "MESH"
    int8_t version;
    int8_t padding[3];
    int32_t vcOffset;
    int32_t vnOffset;
    int32_t uvOffset;
    int32_t facesOffset;
    int32_t textureOffset;
} MeshHeader;

// Padded to be used as an array element in std140 layout uniform blocks
typedef struct {
    float diffuseIntensity;     // 4 bytes
    float specularIntensity;    // 4 bytes
    float specularExponent;     // 4 bytes
    char _padding[4];           // 4 bytes
} MaterialData;                 // 16 bytes total

#pragma pack(pop)

class Mesh: public Renderable, public Movable, public Rotatable, public Scalable {
    public:
        Mesh();
        Mesh(float x, float y, float z);
        Mesh(const Vec3& position);
        ~Mesh();
        
        bool load(const std::string& name);
        
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
        
        void scaleX(float factor);
        void scaleY(float factor);
        void scaleZ(float factor);
        
        float getXFactor() const;
        float getYFactor() const;
        float getZFactor() const;
        
        void scale(float factor);
        
        const Mat4& getScalingMatrix() const {
            return this->scalingMatrix;
        }
        
        void render();
        
    private:
        static const int VERTEX_BUFFER = 0;
        static const int ELEMENT_BUFFER = 1;
        static const int MATERIAL_BUFFER = 2;
        
        Mesh(const Mesh& orig);
        Mesh& operator =(const Mesh&);

        Mat4 translationMatrix;
        Mat4 rotationMatrix;
        Mat4 scalingMatrix;
        
        GLuint buffers[3];
        GLuint vao;
        char* vertexData;
        char* facesData;
        
        void initialize();
};

#endif	/* MESH_H */
