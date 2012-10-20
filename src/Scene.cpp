#include <memory.h>
#include <vector>
#include <set>

#include "Scene.h"
#include "RenderEffect.h"
#include "Renderable.h"
#include "Texture.h"
#include "Logger.h"
#include "configuration.h"

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
    glBufferData(GL_UNIFORM_BUFFER, 16 + sizeof(LightData) * RenderEffect::MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    this->shadowMapEffect = new RenderEffect();
    this->shadowMapEffect->setName("shadowmap");
    this->shadowMapEffect->attachShader("shaders/shadowmap.vs");
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
    if (this->lights.size() < RenderEffect::MAX_LIGHTS) {
        this->lights.push_back(light);
    } else {
        Logger::getInstance().log(Logger::LOG_WARNING, "Maximum light sources count %d reached",
                Scene::MAX_LIGHTS);
    }
}
    
void Scene::render() {
    this->validateLightSources();
    this->shadowRenderPass();
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
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
    
    int i = 0;
    bool updateLightsCount = false;
    std::vector<Light*>::const_iterator light;
    std::set<RenderEffect*>::const_iterator effect;

    for (light = this->lights.begin(); light != this->lights.end(); light++, i++) {
        Light* currentLight = *light;
        
        if (currentLight->isValid() && !currentLight->isRaw()) {
            continue;
        }

        if (currentLight->isRaw()) {
            updateLightsCount = true;
            
            for (effect = this->effects.begin(); effect != this->effects.end(); effect++) {
                RenderEffect* currentEffect = *effect;
                
                switch (currentLight->getType()) {
                    case Light::TYPE_POINT:
                        break;
                        
                    default:
                        currentEffect->setShadowMapSampler(i, i);
                }
            }
        }

        currentLight->setValid(true);
        currentLight->setRaw(false);
        
        LightData data = currentLight->getLightData();
        glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(data) * i, sizeof(data), &data);
    }
    
    if (updateLightsCount) {
        int lightsCount = this->lights.size();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightsCount), &lightsCount);
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::renderToShadowMap(Light* lightSource) {
    this->camera.setProjection(lightSource->getShadowProjection());
    this->camera.setPosition(lightSource->getPosition());

    this->shadowMapEffect->setModelViewPerspectiveMatrix(
            this->camera.getProjection()->getProjectionMatrix() *
            this->camera.getRotationMatrix() *
            this->camera.getTranslationMatrix());

    glClear(GL_DEPTH_BUFFER_BIT);
    
    std::vector<Renderable*>::const_iterator renderable;
    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        Renderable* currentRenderable = *renderable;

        if (currentRenderable->isCastsShadow()) {
            RenderEffect* oldEffect = currentRenderable->getEffect();

            currentRenderable->setEffect(this->shadowMapEffect);
            currentRenderable->render();
            currentRenderable->setEffect(oldEffect);
        }
    }
}

void Scene::normalRenderPass() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    std::vector<Renderable*>::const_iterator renderable;
    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        Renderable* currentRenderable = *renderable;
        RenderEffect* currentEffect = currentRenderable->getEffect();
        
        if (currentEffect != NULL) {
            currentEffect->setModelViewPerspectiveMatrix(
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
    this->depthBuffer.bind();
    glViewport(0, 0, SHADOW, SHADOW);
    
    Vec3 oldPos = this->camera.getPosition();
    Vec3 oldTarget = this->camera.getTargetVector();
    Projection* oldProjection = this->camera.getProjection();
    
    int i = 0;
    std::vector<Light*>::const_iterator light;
    std::set<RenderEffect*>::const_iterator effect;

    for (light = this->lights.begin(); light != this->lights.end(); light++, i++) {
        Light* currentLight = *light;
        
        if (currentLight->isShadow()) {
            switch (currentLight->getType()) {
                case Light::TYPE_POINT:
                    break;

                default:
                    this->depthBuffer.attachTexture(currentLight->getShadowMap());
                    this->camera.lookAt(-currentLight->getDirection());
                    this->renderToShadowMap(currentLight);
                    
                    for (effect = this->effects.begin(); effect != this->effects.end(); effect++) {
                        RenderEffect* currentEffect = *effect;

                        currentEffect->setLightModelViewPerspectiveMatrix(i,
                                this->camera.getProjection()->getProjectionMatrix() *
                                this->camera.getRotationMatrix() *
                                this->camera.getTranslationMatrix());
                    }
                    
                    currentLight->getShadowMap().bind(i);
            }
        }
    }
    
    this->camera.setProjection(oldProjection);
    this->camera.lookAt(oldTarget);
    this->camera.setPosition(oldPos);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
