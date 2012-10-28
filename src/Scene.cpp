#include <memory.h>
#include <vector>
#include <set>

#include "Scene.h"
#include "RenderEffect.h"
#include "Renderable.h"
#include "Texture.h"
#include "Logger.h"
#include "ShadowCubeMapArray.h"
#include "configuration.h"

Scene::Scene():
        ambientLightColor(1.0f, 1.0f, 1.0f) {
    this->ambientLightIntensity = 1.0f;
    
    AmbientLightData ambientLightData;
    memset(&ambientLightData, 0, sizeof(ambientLightData));
    
    ambientLightData.ambientLightIntensity = this->ambientLightIntensity;
    memcpy(ambientLightData.ambientLightColor, this->ambientLightColor.data(),
            sizeof(ambientLightData.ambientLightColor));
    
    int lightsCount = 0;

    glGenBuffers(2, this->buffers);
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::AMBIENT_LIGHT_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ambientLightData), &ambientLightData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
    glBufferData(GL_UNIFORM_BUFFER, 16 + sizeof(LightData) * RenderEffect::MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightsCount), &lightsCount);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    this->depthEffect = new RenderEffect();
    this->depthEffect->setName("shadowmap");
    this->depthEffect->attachShader("shaders/shadowmap.vs");
    this->depthEffect->attachShader("shaders/shadowmap.fs");
    this->effects.insert(this->depthEffect);
    
    this->depthMaps.setDimention(SHADOW);
    this->depthMaps.bind(RenderEffect::SHADOW_MAP_ARRAY_TEXTURE_UNIT);
    
    this->depthPointLightMaps.setDimention(SHADOW);
    this->depthPointLightMaps.bind(RenderEffect::SHADOW_CUBE_MAP_ARRAY_TEXTURE_UNIT);
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
                RenderEffect::MAX_LIGHTS);
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
    
    int index = 0;
    bool updateCount = false;
    Light* currentLight = NULL;
    std::vector<Light*>::const_iterator light;

    for (light = this->lights.begin(); light != this->lights.end(); light++, index++) {
        currentLight = *light;

        if (currentLight->valid && !currentLight->raw) {
            continue;
        }

        if (currentLight->raw) {
            updateCount = true;
        }

        currentLight->valid = true;
        currentLight->raw = false;
        
        LightData data = currentLight->getLightData();
        glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(data) * index, sizeof(data), &data);
    }
    
    if (updateCount) {
        int lightsCount = this->lights.size();
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightsCount), &lightsCount);
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::renderToShadowMap(Light* lightSource) {
    this->camera.setProjection(lightSource->getShadowProjection());
    this->camera.setPosition(lightSource->getPosition());

    this->depthEffect->setModelViewPerspectiveMatrix(
            this->camera.getProjection()->getProjectionMatrix() *
            this->camera.getRotationMatrix() *
            this->camera.getTranslationMatrix());

    RenderEffect* oldEffect = NULL;
    Renderable* currentRenderable = NULL;
    std::vector<Renderable*>::const_iterator renderable;

    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        currentRenderable = *renderable;

        if (currentRenderable->castsShadow()) {
            oldEffect = currentRenderable->getEffect();
            currentRenderable->setEffect(this->depthEffect);
            currentRenderable->render();
            currentRenderable->setEffect(oldEffect);
        }
    }
}

void Scene::normalRenderPass() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderEffect* currentEffect = NULL;
    Renderable* currentRenderable = NULL;
    std::vector<Renderable*>::const_iterator renderable;

    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        currentRenderable = *renderable;

        currentEffect = currentRenderable->getEffect();
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
    glViewport(0, 0, SHADOW, SHADOW);
    
    Vec3 oldPos = this->camera.getPosition();
    Vec3 oldTarget = this->camera.getTarget();
    Projection* oldProjection = new Projection(*this->camera.getProjection());
    
    int index = 0;
    Light* currentLight = NULL;
    RenderEffect* currentEffect = NULL;

    std::vector<Light*>::const_iterator light;
    std::set<RenderEffect*>::const_iterator effect;

    for (light = this->lights.begin(); light != this->lights.end(); light++, index++) {
        currentLight = *light;

        if (currentLight->castsShadow()) {
            switch (currentLight->getType()) {
                case Light::TYPE_POINT:
                    this->depthPointLightBuffer.bind();
                    this->depthEffect->setCameraPosition(currentLight->getPosition());
                    
                    glClearColor(1000.0f, 0.0f, 0.0f, 0.0f);     // TODO: sync with zFar
                    glDrawBuffer(GL_BACK);
                    
                    for (int i = 0; i < 6; i++) {
                        this->depthPointLightBuffer.attachColorTexture(this->depthPointLightMaps, index * 6 + i);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        // Due to gimbal lock we have an infine number of
                        // lookAt(Vec3::UNIT_Y) results. Final camera target
                        // depends on initial target, thus for proper cubemap
                        // POSITIVE_Y and NEGATIVE_Y faces rendering we have to
                        // reset camera target to Vec3::UNIT_Z direction.
                        this->camera.lookAt(Vec3::UNIT_Z);
                        this->camera.lookAt(ShadowCubeMapArray::getFaceDirection(i));
                        this->renderToShadowMap(currentLight);
                    }
                    break;

                default:
                    this->depthBuffer.bind();
                    this->depthBuffer.attachDepthTexture(this->depthMaps, index);
                    
                    glDrawBuffer(GL_NONE);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    
                    this->camera.lookAt(currentLight->getDirection());
                    this->renderToShadowMap(currentLight);
                    
                    for (effect = this->effects.begin(); effect != this->effects.end(); effect++) {
                        currentEffect = *effect;
                        currentEffect->setLightModelViewPerspectiveMatrix(index,
                                this->camera.getProjection()->getProjectionMatrix() *
                                this->camera.getRotationMatrix() *
                                this->camera.getTranslationMatrix());
                    }
            }
        }
    }
    
    this->camera.setProjection(oldProjection);
    this->camera.lookAt(oldTarget);
    this->camera.setPosition(oldPos);
}
