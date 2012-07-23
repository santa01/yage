#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <ctime>
#include <string>
#include <vector>

class OpenGLWindow {
    public:
        static const int KEY_PRESSED = 0;
        static const int KEY_RELEASED = 1;
        static const int BUTTON_PRESSED = 2;
        static const int BUTTON_RELEASED = 3;

        OpenGLWindow();
        OpenGLWindow(int width, int height, const std::string& caption);
        virtual ~OpenGLWindow();

        void getKeysStates(std::vector<bool>& keysStates) const;
        void captureMouse(bool capture);
        
        bool isMouseCaptured() const {
            return this->captured;
        }
        
        float getFrameTime() const {
            return this->frameTime;
        }

        void swapBuffers() {
            glXSwapBuffers(this->display, this->window);
        }
        
        void terminate() {
            this->running = false;
        }

        void run();

    protected:
        virtual void onMouseMotionEvent(int x, int y) = 0;
        virtual void onMouseButtonEvent(int x, int y, int button, int state) = 0;
        virtual void onKeyEvent(int state, int keycode) = 0;
        virtual void onInit() = 0;
        virtual void onCleanup() = 0;
        virtual void onIdle() = 0;

        std::string caption;
        int width;
        int height;
        
    private:
        Window window;
        Display *display;
        GLXContext glxContext;
        Colormap colormap;
        
        Atom wmDeleteWindow;
        Atom wmCaptureMouse;

        int oldX, oldY;
        bool captured;
        bool running;
        float frameTime;
        
        OpenGLWindow(const OpenGLWindow&);
        OpenGLWindow& operator =(const OpenGLWindow&);

        void initWindow();
        void destroyWindow();
};

#endif /* OPENGLWINDOW_H */
