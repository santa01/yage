#ifndef SKYBOX_H
#define SKYBOX_H

#include "Renderable.h"
#include "Movable.h"
#include "Scalable.h"
#include "Vec3.h"
#include "Mat4.h"

class SkyBox: public Renderable, public Movable, public Scalable {
    public:
        SkyBox();
        SkyBox(float x, float y, float z);
        SkyBox(const Vec3& position);
        ~SkyBox();
      
        using Movable::setPosition;
        
        void setPosition(const Vec3& position);
        Vec3 getPosition() const;
        
        void scaleX(float factor) {
            this->scalingMatrix.set(0, 0, factor);
        }
        
        void scaleY(float factor) {
            this->scalingMatrix.set(1, 1, factor);
        }
        
        void scaleZ(float factor) {
            this->scalingMatrix.set(2, 2, factor);
        }
        
        float getXFactor() const {
            return this->scalingMatrix.get(0, 0);
        }
        
        float getYFactor() const {
            return this->scalingMatrix.get(1, 1);
        }
        
        float getZFactor() const {
            return this->scalingMatrix.get(2, 2);
        }

        void scale(float factor);

        void render();  

    private:
        enum {
            VERTEX_BUFFER,
            ELEMENT_BUFFER
        };

        SkyBox(const SkyBox& orig);
        SkyBox& operator =(const SkyBox&);

        Mat4 translationMatrix;
        Mat4 scalingMatrix;
        
        GLuint buffers[2];
        GLuint vao;
        
        static const float vertices[];
        static const int indices[];
        
        void initialize();
};

#endif /* SKYBOX_H */
