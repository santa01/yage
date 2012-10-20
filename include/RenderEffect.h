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
        static const int MAX_LIGHTS = 24;
        static const int DIFFUSE_TEXTURE_UNIT = 25;
        static const int SPECULAR_TEXTURE_UNIT = 26;
        
        RenderEffect();

        void setName(const std::string& name) {
            this->name = name;
        }
        
        const std::string& getName() const {
            return this->name;
        }
        
        void setModelViewPerspectiveMatrix(const Mat4& matrix);
        void setLocalWorldMatrix(const Mat4& matrix);
        void setLightModelViewPerspectiveMatrix(int lightIndex, const Mat4& matrix);
        
        void setShadowMapSampler(int lightIndex, int textureUnit);
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

        std::vector<GLuint> shaderList;
        std::string name;
        
        GLuint program;
        
        GLint modelViewPerspectiveMatrix;       // vertex shader
        GLint localWorldMatrix;                 // vertex shader
        GLint cameraPosition;                   // fragment shader

        GLint lightModelViewPerspectiveMatrices[RenderEffect::MAX_LIGHTS];   // vertex shader
        GLint shadowMapSamplers[RenderEffect::MAX_LIGHTS];                   // fragment shader
};

#endif	/* RENDEREFFECT_H */
