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
            BUTTON_RELEASED
        };

        OpenGLWindow();
        OpenGLWindow(int width, int height, const std::string& caption, bool fullScreen);
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
            // Checked in RRScreenChangeNotify event handler
            this->terminateRequest = true;
        }

        void run();

    protected:
        virtual void onMouseMotionEvent(int x, int y) = 0;
        virtual void onMouseButtonEvent(int x, int y, int button, int state) = 0;
        virtual void onKeyEvent(int state, int keycode) = 0;
        virtual void onInit() = 0;
        virtual void onCleanup() = 0;
        virtual void onIdle() = 0;
        
    protected:
        std::string caption;
        int width;
        int height;
        bool fullScreen;

    private:
        Window window, rootWindow;
        Display *display;
        GLXContext glxContext;
        GLXFBConfig fbConfig;
        XVisualInfo *visualInfo;
        Colormap colormap;
        SizeID resolutionID, oldResolutionID;

        Atom wmDeleteWindow;
        Atom wmCaptureMouse;
        Atom wmState;
        Atom wmFullScreen;

        int xrandrEventBase;
        int oldX, oldY;
        float frameTime;

        bool captured;
        bool running;
        bool msaaEnabled;
        bool nativeResolutuionEnabled;
        bool terminateRequest;
        bool toggleFullScreenRequest;
        
        OpenGLWindow(const OpenGLWindow&);
        OpenGLWindow& operator =(const OpenGLWindow&);

        void initialize();
        void initWindow();
        void destroyWindow();

        bool checkSupportedResolutions();
        bool checkFrameBufferConfigs();
        void initVSync();
        void initOpenGL();

        void toggleFullScreen();
};

#endif /* OPENGLWINDOW_H */
