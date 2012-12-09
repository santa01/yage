#include <memory.h>
#include <vector>
#include <set>

#include "Scene.h"
#include "RenderEffect.h"
#include "Renderable.h"
#include "Texture.h"
#include "Logger.h"

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
    std::vector<Renderable*>::const_iterator renderable;
    for (renderable = this->renderables.begin(); renderable != this->renderables.end(); renderable++) {
        Renderable* currentRenderable = *renderable;
        RenderEffect* currentEffect = currentRenderable->getEffect();
        
        if (currentEffect != NULL) {
            currentEffect->setMVPMatrix(
                    this->camera.getPerspectiveMatrix() *
                    this->camera.getRotationMatrix() *
                    this->camera.getTranslationMatrix());
            currentEffect->setCameraPosition(this->camera.getPosition());

            currentEffect->setAmbientLight(this->buffers[Scene::AMBIENT_LIGHT_BUFFER]);
            currentEffect->setLightSources(this->buffers[Scene::LIGHT_SOURCES_BUFFER]);
        }

        currentRenderable->render();
    }
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
