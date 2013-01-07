#include <string>

#include "Renderer.h"
#include "ShaderLoader.h"
#include "Mat4.h"
#include "Logger.h"
#include "RenderEffect.h"
#include "TGAImage.h"
#include "ImageTexture.h"
#include "Mesh.h"
#include "Renderable.h"
#include "Scene.h"
#include "DirectedLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SkyBox.h"
#include "CubeTexture.h"
#include "Extention.h"
#include "configuration.h"

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

        this->scene->getCamera().rotate(Vec3::UNIT_Y, x * MOUSE_SPEED);
        if (yAngle != 0) {
            this->scene->getCamera().rotate(this->scene->getCamera().getRight(), yAngle);
        }
    }
}

void Renderer::onMouseButtonEvent(int x, int y, int button, int state) {
}

void Renderer::onKeyEvent(int state, int keycode) {
}

void Renderer::onInit() {
    this->scene = new Scene();

    float anisotropy = 0.0f;
    if (Extention::isSupported("GL_EXT_texture_filter_anisotropic")) {
        Logger::getInstance().log(Logger::LOG_INFO, "GL_EXT_texture_filter_anisotropic supported");
        
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        anisotropy = (ANISOTROPY > anisotropy) ? anisotropy : ANISOTROPY;
        Logger::getInstance().log(Logger::LOG_INFO, "  textures will have anisotropic filtering level %f", anisotropy);
    }

    TGAImage image;
    image.load("textures/metal.tga");

    ImageTexture* texture = new ImageTexture(anisotropy);
    texture->load(image);
    
    RenderEffect* effect = new RenderEffect();
    effect->setName("generic_illumination");
    effect->attachShader("shaders/generic_illumination.vs");
    effect->attachShader("shaders/generic_illumination.fs");
    
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
        mesh->setName("cube4");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->scale(10.0f);
        mesh->scaleZ(0.01f);
        mesh->setPosition(0.0f, 5.0f, 8.0f);
        this->scene->addRenderable(mesh);
    }
    
    mesh = new Mesh();
    if (mesh->load("meshes/cube.mesh")) {
        mesh->setName("cube1");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->setPosition(3.0f, 0.0f, 0.0f);
        mesh->roll(45.0f);
        mesh->setCastsShadow(true);
        this->scene->addRenderable(mesh);
    }

    mesh = new Mesh();
    if (mesh->load("meshes/cube.mesh")) {
        mesh->setName("cube2");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->setPosition(3.0f, 2.0f, 4.0f);
        mesh->roll(20.0f);
        mesh->setCastsShadow(true);
        this->scene->addRenderable(mesh);
    }
    
    mesh = new Mesh();
    if (mesh->load("meshes/cube.mesh")) {
        mesh->setName("cube3");
        mesh->setDiffuseTexture(texture);
        mesh->setEffect(effect);
        mesh->setPosition(0.0f, 0.0f, 2.0f);
        mesh->roll(85.0f);
        mesh->yaw(30.0f);
        mesh->setCastsShadow(true);
        this->scene->addRenderable(mesh);
    }
    
    RenderEffect* effect1 = new RenderEffect();
    effect1->setName("skybox");
    effect1->attachShader("shaders/skybox.vs");
    effect1->attachShader("shaders/skybox.fs");

    CubeTexture* skybox = new CubeTexture(anisotropy);

    image.load("textures/sky/front.tga");
    skybox->loadFaceImage(CubeTexture::FRONT_FACE, image);
    
    image.load("textures/sky/back.tga");
    skybox->loadFaceImage(CubeTexture::BACK_FACE, image);
    
    image.load("textures/sky/right.tga");
    skybox->loadFaceImage(CubeTexture::RIGHT_FACE, image);
    
    image.load("textures/sky/left.tga");
    skybox->loadFaceImage(CubeTexture::LEFT_FACE, image);
    
    image.load("textures/sky/top.tga");
    skybox->loadFaceImage(CubeTexture::TOP_FACE, image);
    
    image.load("textures/sky/bottom.tga");
    skybox->loadFaceImage(CubeTexture::BOTTOM_FACE,image);
    
    SkyBox* sky = new SkyBox();
    sky->setName("skybox");
    sky->setEffect(effect1);
    sky->setDiffuseTexture(skybox);
    this->scene->addRenderable(sky);
    this->scene->getCamera().setSkyBox(sky);

    PointLight* light1 = new PointLight();
    light1->setPosition(1.0f, 3.0f, 1.0f);
    light1->setEnergy(1.0f);
    light1->setFalloff(10.0f);
    
    PointLight* light5 = new PointLight();
    light5->setPosition(2.0f, 3.0f, 1.0f);
    light5->setEnergy(1.0f);
    light5->setFalloff(10.0f);
    this->scene->addLight(light5);
    
    SpotLight* light2 = new SpotLight();
    light2->setPosition(2.0f, 4.0f, -4.0f);
    light2->setSize(50.0f);
    light2->pitch(-70.0f);
    light2->setEnergy(2.0f);
    this->scene->addLight(light2);
    this->scene->addLight(light1);
    
    DirectedLight* light3 = new DirectedLight();
    light3->setDirection(0.0f, -5.0f, 5.0f);
    light3->setEnergy(0.1f);
    //this->scene->addLight(light3);
    
    DirectedLight* light4 = new DirectedLight();
    light4->setDirection(-5.0f, -5.0f, 5.0f);
    light4->setEnergy(0.1f);
    this->scene->addLight(light4);
    
    this->scene->setAmbientLightIntensity(0.0f);
    this->scene->getCamera().setPosition(1.5f, 1.5f, -5.0f);
    PerspectiveProjection* projection =
            (PerspectiveProjection*)this->scene->getCamera().getProjection();
    projection->setAspectRatio((float)this->width / this->height);

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
        cameraPosition += this->scene->getCamera().getTarget() *
                          FORWARD_SPEED * corrention;
    }

    if (this->keysStates[BACKWARD]) {
        cameraPosition -= this->scene->getCamera().getTarget() *
                          BACKWARD_SPEED  * corrention;
    }

    if (this->keysStates[STEP_RIGHT]) {
        cameraPosition += this->scene->getCamera().getRight() *
                          STEP_SPEED  * corrention;
    }
    
    if (this->keysStates[STEP_LEFT]) {
        cameraPosition -= this->scene->getCamera().getRight() *
                          STEP_SPEED  * corrention;
    }

//    if (this->keyStates[JUMP])
//    if (this->keyStates[CROUCH])

    this->scene->getCamera().setPosition(cameraPosition);
    this->scene->render();
    
    this->swapBuffers();
}
