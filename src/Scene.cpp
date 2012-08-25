#include <memory.h>
#include <vector>
#include <set>

#include "Scene.h"
#include "RenderEffect.h"
#include "Renderable.h"
#include "Texture.h"
#include "Logger.h"
#include "PerspectiveProjection.h"
#include "configuration.h"
#include "SpotLight.h"

Scene::Scene():
        ambientLightColor(1.0f, 1.0f, 1.0f) {
    this->ambientLightIntensity = 1.0f;
    
    AmbientLightData ambientLightData;
    memset(&ambientLightData, 0, sizeof(ambientLightData));
    
    ambientLightData.ambientLightIntensity = this->ambientLightIntensity;
    memcpy(ambientLightData.ambientLightColor, this->ambientLightColor.data(),
            sizeof(ambientLightData.ambientLightColor));

    glGenBuffers(2, this->buffers);
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::AMBIENT_LIGHT_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ambientLightData), &ambientLightData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, 16 + sizeof(LightData) * Scene::MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    this->shadowMapEffect = new RenderEffect();
    this->shadowMapEffect->attachShader("shaders/mvp_lightsource.vs");
    this->effects.insert(this->shadowMapEffect);
}

Scene::~Scene() {
    std::vector<Renderable*>::const_iterator renderable;
    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        delete *renderable;
    }

    std::vector<Light*>::const_iterator light;
    for (light = this->lights.begin(); light != this->lights.end(); light++) {
        delete *light;
    }
    
    std::set<RenderEffect*>::const_iterator effect;
    for (effect = this->effects.begin(); effect != this->effects.end(); effect++) {
        delete *effect;
    }
    
    std::set<Texture*>::const_iterator texture;
    for (texture = this->textures.begin(); texture != this->textures.end(); texture++) {
        delete *texture;
    }
    
    glDeleteBuffers(2, this->buffers);
}

bool Scene::load(const std::string& /*name*/) {
    // TODO: implement
    return false;
}

void Scene::setAmbientLightColor(const Vec3& color) {
    this->ambientLightColor = color;
    this->updateAmbientLightBuffer();
}

void Scene::setAmbientLightIntensity(float intensity) {
    this->ambientLightIntensity = intensity;
    this->updateAmbientLightBuffer();
}

void Scene::addRenderable(Renderable* entity) {
    this->renderables.push_back(entity);
    this->effects.insert(entity->getEffect());
    this->textures.insert(entity->getDiffuseTexture());
}

void Scene::addLight(Light* light) {
    if (this->lights.size() < Scene::MAX_LIGHTS) {
        this->lights.push_back(light);
        
        int lightsCount = this->lights.size();
        LightData data = light->getLightData();
        
        glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightsCount), &lightsCount);
        glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(data) * --lightsCount, sizeof(data), &data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    } else {
        Logger::getInstance().log(Logger::LOG_WARNING, "Maximum light sources count %d reached",
                Scene::MAX_LIGHTS);
    }
}

void Scene::render() {
    this->validateLightSources();
    this->shadowRenderPass();
    this->depthBuffer.bindShadowMap(2);
    this->normalRenderPass();
}

void Scene::updateAmbientLightBuffer() {
    AmbientLightData ambientLightData;
    memset(&ambientLightData, 0, sizeof(ambientLightData));
    
    ambientLightData.ambientLightIntensity = this->ambientLightIntensity;
    memcpy(ambientLightData.ambientLightColor, this->ambientLightColor.data(),
            sizeof(ambientLightData.ambientLightColor));
    
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::AMBIENT_LIGHT_BUFFER]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ambientLightData), &ambientLightData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::validateLightSources() {
    std::vector<Light*>::const_iterator light;
    int lightIndex = 0;
    for (light = this->lights.begin(); light != this->lights.end(); light++) {
        Light* currentLight = *light;
        
        if (!currentLight->isValid()) {
            LightData data = currentLight->getLightData();

            glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
            glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(data) * lightIndex, sizeof(data), &data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            
            currentLight->setValid(true);
        }
        
        lightIndex++;
    }
}

void Scene::normalRenderPass() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 800, 600);
    
    std::vector<Renderable*>::const_iterator renderable;
    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        Renderable* currentRenderable = *renderable;
        RenderEffect* currentEffect = currentRenderable->getEffect();
        
        if (currentEffect != NULL) {
            currentEffect->setMVPMatrix(
                    this->camera.getProjection()->getProjectionMatrix() *
                    this->camera.getRotationMatrix() *
                    this->camera.getTranslationMatrix());
            currentEffect->setCameraPosition(this->camera.getPosition());

            currentEffect->setAmbientLight(this->buffers[Scene::AMBIENT_LIGHT_BUFFER]);
            currentEffect->setLightSources(this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
        }

        currentRenderable->render();
    }
}

void Scene::shadowRenderPass() {
    this->depthBuffer.attach();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1024, 1024);

    Vec3 oldPos = this->camera.getPosition();
    Vec3 oldTarget = -this->camera.getTargetVector();
    
    std::vector<Light*>::const_iterator light;
    for (light = this->lights.begin(); light != this->lights.end(); light++) {
        Light* currentLight = *light;
        
        if (currentLight->isShadow()) {
            switch (currentLight->getType()) {
                case Light::TYPE_SPOT:
                    SpotLight* spotLight = (SpotLight*)currentLight;
                    this->camera.setPosition(spotLight->getPosition());
                    this->camera.lookAt(spotLight->getDirection());
                    PerspectiveProjection* proj = (PerspectiveProjection*)this->camera.getProjection();
                    proj->setFov(spotLight->getSize());
                    proj->setAspectRatio(1.0f);
            }
            
            this->shadowMapEffect->setMVPMatrix(
                    this->camera.getProjection()->getProjectionMatrix() *
                    this->camera.getRotationMatrix() *
                    this->camera.getTranslationMatrix());
            
            std::vector<Renderable*>::const_iterator renderable;
            for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
                Renderable* currentRenderable = *renderable;
                
                if (currentRenderable->isCastsShadow()) {
                    RenderEffect* oldEffect = currentRenderable->getEffect();
                    oldEffect->setLightMVPMatrix(
                        this->camera.getProjection()->getProjectionMatrix() *
                        this->camera.getRotationMatrix() *
                        this->camera.getTranslationMatrix());

                    currentRenderable->setEffect(this->shadowMapEffect);
                    currentRenderable->render();
                    currentRenderable->setEffect(oldEffect);
                }
            }
        }
    }

    this->camera.setPosition(oldPos);
    this->camera.lookAt(oldTarget);
    PerspectiveProjection* proj = (PerspectiveProjection*)this->camera.getProjection();
    proj->setFov(FOV);
    proj->setAspectRatio(1.3333f);
}
