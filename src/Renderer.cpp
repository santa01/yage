#include <string>

#include "Renderer.h"
#include "ShaderLoader.h"
#include "Mat4.h"
#include "Logger.h"
#include "RenderEffect.h"
#include "TGAImage.h"
#include "Texture.h"
#include "Mesh.h"
#include "Renderable.h"
#include "Scene.h"
#include "DirectedLight.h"
#include "configuration.h"
#include "PointLight.h"
#include "SpotLight.h"

Renderer::Renderer():
        OpenGLWindow() {
    this->cameraVerticalAngle = 0.0f;
}

Renderer::Renderer(int width, int height, const std::string& caption, bool fullScreen):
        OpenGLWindow(width, height, caption, fullScreen) {
    this->cameraVerticalAngle = 0.0f;
}

void Renderer::onMouseMotionEvent(int x, int y) {
    if (this->isMouseCaptured()) {
        float yAngle = y * MOUSE_SPEED;

        if (this->cameraVerticalAngle + yAngle > 90.0f) {
            yAngle = 90.0f - this->cameraVerticalAngle;
        } else if (this->cameraVerticalAngle + yAngle < -90.0f) {
            yAngle = -90.0f - this->cameraVerticalAngle;
        }

        this->cameraVerticalAngle += yAngle;

        this->scene->getCamera().rotate(Vec3(0.0f, 1.0f, 0.0f), x * MOUSE_SPEED);
        if (yAngle != 0) {
            this->scene->getCamera().rotate(this->scene->getCamera().getRightVector(), yAngle);
        }
    }
}

void Renderer::onMouseButtonEvent(int x, int y, int button, int state) {
}

void Renderer::onKeyEvent(int state, int keycode) {
}

void Renderer::onInit() {
    this->scene = new Scene();

    TGAImage image;
    image.load("textures/metal.tga");

    Texture* texture = new Texture();
    texture->load(image);
    
    RenderEffect* effect = new RenderEffect();
    effect->setName("illumination");
    effect->attachShader("shaders/mvp_transform.vs");
    effect->attachShader("shaders/lambert_phong_shading.fs");
    
    Mesh* mesh = new Mesh();
    if (mesh->load("meshes/cube.mesh")) {
        mesh->setName("cube");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->scale(10.0f);
        mesh->scaleY(0.01f);
        this->scene->addRenderable(mesh);
    }
    
    mesh = new Mesh();
    if (mesh->load("meshes/cube.mesh")) {
        mesh->setName("cube1");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->setPosition(3.0f, 0.0f, 0.0f);
        mesh->roll(45.0f);
        this->scene->addRenderable(mesh);
    }

    PointLight* light1 = new PointLight();
    light1->setPosition(5.0f, 5.0f, -5.0f);
    light1->setEnergy(3.0f);
    light1->setFalloff(10.0f);
    //this->scene->addLight(light1);
    
    SpotLight* light2 = new SpotLight();
    light2->setPosition(5.0f, 5.0f, 5.0f);
    light2->pitch(35.0f);
    this->scene->addLight(light2);
    
    this->scene->setAmbientLightIntensity(0.2f);
    this->scene->getCamera().setPosition(1.5f, 1.5f, -5.0f);
    this->scene->getCamera().setAspectRatio((float)this->width / this->height);

    this->captureMouse(true);
}

void Renderer::onCleanup() {
    if (this->scene) {
        delete this->scene;
    }
}

void Renderer::onIdle() {
    this->getKeysStates(this->keysStates);

    if (this->keysStates[EXIT]) {
        this->terminate();
    }
    
    if (this->keysStates[10]) {         // "1"
        this->captureMouse(true);
    }
    
    if (this->keysStates[11]) {         // "2"
        this->captureMouse(false);
    }

    Vec3 cameraPosition = this->scene->getCamera().getPosition();
    float corrention = this->getFrameTime();

    if (this->keysStates[FORWARD]) {
        cameraPosition += this->scene->getCamera().getTargetVector() *
                          FORWARD_SPEED * corrention;
    }

    if (this->keysStates[BACKWARD]) {
        cameraPosition -= this->scene->getCamera().getTargetVector() *
                          BACKWARD_SPEED  * corrention;
    }

    if (this->keysStates[STEP_RIGHT]) {
        cameraPosition += this->scene->getCamera().getRightVector() *
                          STEP_SPEED  * corrention;
    }
    
    if (this->keysStates[STEP_LEFT]) {
        cameraPosition -= this->scene->getCamera().getRightVector() *
                          STEP_SPEED  * corrention;
    }

//    if (this->keyStates[JUMP])
//    if (this->keyStates[CROUCH])

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->scene->getCamera().setPosition(cameraPosition);
    this->scene->render();
    
    this->swapBuffers();
}
