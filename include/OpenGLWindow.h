#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <global.h>
#include <ctime>
#include <string>
#include <vector>

#if UNIX
# include <X11/Xlib.h>
# include <X11/extensions/Xrandr.h>
#endif

class OpenGLWindow {
    public:
        enum {
            KEY_PRESSED,
            KEY_RELEASED,
            BUTTON_PRESSED,
            BUTTON_RELEASED,
        };

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

        void initialize();
        
        void initWindow();
        void destroyWindow();
};

#endif /* OPENGLWINDOW_H */
