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
        static const int DIFFUSE_TEXTURE_UNIT = 0;
        static const int SPECULAR_TEXTURE_UNIT = 1;
        
        RenderEffect();

        void setName(const std::string& name) {
            this->name = name;
        }
        
        const std::string& getName() const {
            return this->name;
        }
        
        void setMVPMatrix(const Mat4& mvpMatrix);
        void setMVMatrix(const Mat4& mvMatrix);
        void setLightMVPMatrix(const Mat4& lightSpaceMvpMatrix);
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
        
        GLint mvp;                              // vertex shader
        GLint mv;                               // vertex shader
        GLint lightSpaceMvp;                    // vertex shader
        GLint cameraPosition;                   // fragment shader
        GLint textureSampler;                   // fragment shader
};

#endif	/* RENDEREFFECT_H */
