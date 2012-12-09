#ifndef RENDERER_H
#define RENDERER_H

#include <global.h>
#include <vector>
#include <string>

#include "OpenGLWindow.h"
#include "Scene.h"
#include "Mesh.h"

class Renderer: public OpenGLWindow {
    public:
        Renderer();
        Renderer(int width, int height, const std::string& caption, bool fullScreen);
    
    private:
        void onMouseMotionEvent(int x, int y);
        void onMouseButtonEvent(int state, int x, int y, int button);
        void onKeyEvent(int state, int keycode);
        void onInit();
        void onCleanup();
        void onIdle();
        
        Scene* scene;
        
        std::vector<bool> keysStates;
        float cameraVerticalAngle;
};

#endif /* RENDERER_H */
