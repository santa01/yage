#ifndef RENDEREFFECT_H
#define	RENDEREFFECT_H

#include <global.h>
#include <vector>
#include <string>

#include "Mat4.h"
#include "Vec3.h"
#include "Light.h"

class RenderEffect {
    public:
        static const unsigned int MAX_LIGHTS = 16;
        static const int SHADOW_MAP_ARRAY_TEXTURE_UNIT = 0;
        static const int SHADOW_CUBE_MAP_ARRAY_TEXTURE_UNIT = 1;
        static const int DIFFUSE_TEXTURE_UNIT = 2;
        static const int SPECULAR_TEXTURE_UNIT = 3;
        
        RenderEffect();
        ~RenderEffect();

        void setName(const std::string& name) {
            this->name = name;
        }
        
        const std::string& getName() const {
            return this->name;
        }
        
        void setModelViewPerspectiveMatrix(const Mat4& matrix);
        void setLocalWorldMatrix(const Mat4& matrix);
        void setLightModelViewPerspectiveMatrix(int lightIndex, const Mat4& matrix);

        void setCameraPosition(const Vec3& cameraPosition);

        void setMaterial(GLuint materialBuffer);
        void setAmbientLight(GLuint ambientLightBuffer);
        void setLightSources(GLuint lightSourcesBuffer);
        
        void attachShader(const std::string& name);
        void enable();
       
    private:
        static const int MATERIAL_PARAMETERS_BINDPOINT = 0;
        static const int AMBIENT_LIGHT_BINDPOINT = 1;
        static const int LIGHT_SOURCES_BINDPOINT = 2;
        
        RenderEffect(const RenderEffect& orig);
        RenderEffect& operator =(const RenderEffect&);

        std::vector<std::string> shaderList;
        std::string name;
        
        GLuint program;
        
        GLint modelViewPerspectiveMatrix;       // vertex shader
        GLint localWorldMatrix;                 // vertex shader
        GLint cameraPosition;                   // fragment shader

        GLint lightModelViewPerspectiveMatrices[RenderEffect::MAX_LIGHTS];   // vertex shader
};

#endif	/* RENDEREFFECT_H */
