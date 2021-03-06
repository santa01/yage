#ifndef SCENE_H
#define	SCENE_H

#include <global.h>
#include <vector>
#include <string>
#include <set>

#include "Renderable.h"
#include "Texture.h"
#include "RenderEffect.h"
#include "Light.h"
#include "Camera.h"
#include "Vec3.h"
#include "FrameBuffer.h"
#include "ShadowMapArray.h"
#include "ShadowCubeMapArray.h"

#pragma pack(push, 1)

typedef struct {
    float ambientLightColor[3];         // 12 bytes, aligned to vec4 size (16 bytes)
    float ambientLightIntensity;        // 4 bytes
} AmbientLightData;                     // 16 bytes total

#pragma pack(pop)

class Scene {
    public:
        Scene();
        ~Scene();
        
        bool load(const std::string& name);
        
        void setAmbientLightColor(const Vec3& color);
        const Vec3& getAmbientLightColor() const {
            return this->ambientLightColor;
        }
        
        void setAmbientLightIntensity(float intensity);
        float getAmbientLightIntensity() const {
            return this->ambientLightIntensity;
        }
        
        Camera& getCamera() {
            return this->camera;
        }
        
        void setCamera(const Camera& camera) {
            this->camera = camera;
        }
        
        void addRenderable(Renderable* entity);
        void addLight(Light* light);
        
        void render();
    
    private:
        static const int AMBIENT_LIGHT_BUFFER = 0;
        static const int LIGHT_SOURCES_BUFFER = 1;
        
        Scene(const Scene&);
        Scene& operator =(const Scene&);
        
        void updateAmbientLightBuffer();
        void validateLightSources();
        void renderToShadowMap(Light* lightSource);
        
        void normalRenderPass();
        void shadowRenderPass();
        
        std::vector<Renderable*> renderables;
        std::vector<Light*> lights;
        std::set<RenderEffect*> effects;
        std::set<Texture*> textures;

        Camera camera;
        
        FrameBuffer depthBuffer;
        FrameBuffer depthPointLightBuffer;

        ShadowMapArray depthMaps;
        ShadowCubeMapArray depthPointLightMaps;

        RenderEffect* depthEffect;
        
        Vec3 ambientLightColor;
        float ambientLightIntensity;
        
        GLuint buffers[2];
};

#endif	/* SCENE_H */
