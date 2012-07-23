#define GLX_GLXEXT_PROTOTYPES
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <sys/time.h>
#include <memory.h>
#include <cstdlib>
#include <string>
#include <vector>

#include "OpenGLWindow.h"
#include "Logger.h"
#include "Extention.h"
#include "configuration.h"

OpenGLWindow::OpenGLWindow() {
    this->width = 640;
    this->height = 480;
    this->captured = false;
    this->running = true;
    this->frameTime = 0.0f;
}

OpenGLWindow::OpenGLWindow(int width, int height, const std::string& caption):
        caption(caption) {
    this->width = width;
    this->height = height;
    this->captured = false;
    this->running = true;
    this->frameTime = 0.0f;
}

OpenGLWindow::~OpenGLWindow() {
}

void OpenGLWindow::getKeysStates(std::vector<bool>& keysStates) const {
    char keys[32];
    XQueryKeymap(this->display, keys);
    
    keysStates.clear();
    for (int i = 0; i < 256; i++) {
        keysStates.push_back((keys[i >> 3] & (1 << i % 8)) ? true : false);
    }    
}

void OpenGLWindow::captureMouse(bool capture) {
    if (capture) {
        XColor color;
        char data[8];
        
        memset(&color, 0, sizeof(color));
        memset(data, 0, sizeof(data));
        Pixmap pixmap = XCreateBitmapFromData(this->display, this->window, data, 1, 1);
        Cursor cursor = XCreatePixmapCursor(this->display, pixmap, pixmap, &color, &color, 0, 0);

        XFreePixmap(this->display, pixmap);        
        XDefineCursor(this->display, this->window, cursor);
    } else {
        XUndefineCursor(this->display, this->window);
    }
    
    XClientMessageEvent captureEvent;
    captureEvent.type = ClientMessage;
    captureEvent.format = 32;
    captureEvent.data.l[0] = this->wmCaptureMouse;
    captureEvent.data.l[1] = capture;

    XWarpPointer(this->display, None, this->window, 0, 0, 0, 0,
        (this->width >> 1), (this->height >> 1));

    XSendEvent(this->display, this->window, True, NoEventMask, (XEvent*)&captureEvent);
}

void OpenGLWindow::run() {
    XEvent event;
    struct timeval startFrameTime, endFrameTime;

    Logger::getInstance().log(Logger::LOG_INFO, "initializing");
    this->initWindow();
    this->onInit();

    while (this->running) {
        gettimeofday(&startFrameTime, NULL);

        while (XPending(this->display)) {
            XNextEvent(this->display, &event);

            switch (event.type) {
                case KeyPress:
                    this->onKeyEvent(OpenGLWindow::KEY_PRESSED, event.xkey.keycode);
                    break;

                case KeyRelease:
                    this->onKeyEvent(OpenGLWindow::KEY_RELEASED, event.xkey.keycode);
                    break;

                case ButtonPress:
                    this->onMouseButtonEvent(event.xbutton.x, event.xbutton.y,
                            event.xbutton.button, OpenGLWindow::BUTTON_PRESSED);
                    break;

                case ButtonRelease:
                    this->onMouseButtonEvent(event.xbutton.x, event.xbutton.y,
                            event.xbutton.button, OpenGLWindow::BUTTON_RELEASED);
                    break;

                case MotionNotify:
                    if (this->captured) {
                        this->onMouseMotionEvent(event.xmotion.x - (this->width >> 1),
                            event.xmotion.y - (this->height >> 1));
                    } else {
                        this->onMouseMotionEvent(event.xmotion.x, event.xmotion.y);
                    }

                    break;

                case ClientMessage:
                    if (event.xclient.data.l[0] == (long)this->wmDeleteWindow) {
                        this->running = false;
                    } else if (event.xclient.data.l[0] == (long)this->wmCaptureMouse) {
                        this->captured = event.xclient.data.l[1];
                    }

                    break;
            }
        }
        
        if (this->captured) {
            XWarpPointer(this->display, None, this->window, 0, 0, 0, 0,
                    (this->width >> 1), (this->height >> 1));
        }
        
        this->onIdle();

        gettimeofday(&endFrameTime, NULL);
        this->frameTime = (endFrameTime.tv_sec - startFrameTime.tv_sec) +
                (endFrameTime.tv_usec - startFrameTime.tv_usec) / 1000000.0f;
    }
    
    Logger::getInstance().log(Logger::LOG_INFO, "cleaning up");
    this->onCleanup();
    this->destroyWindow();
}

void OpenGLWindow::initWindow() {
    this->display = XOpenDisplay(NULL);
    if (!this->display) {
        Logger::getInstance().log(Logger::LOG_ERROR, "cannot open display");
        exit(EXIT_FAILURE);
    }

    int defaultScreen = XDefaultScreen(this->display);
    Window rootWindow = XRootWindow(this->display, defaultScreen);

    int fbAttributes[] = {
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        GLX_X_RENDERABLE, True,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        None
    };
    
    int fbConfigsCount = 0;    
    GLXFBConfig *fbConfigs = glXChooseFBConfig(this->display,
            DefaultScreen(this->display), fbAttributes, &fbConfigsCount);
    if (fbConfigs == NULL) {
        Logger::getInstance().log(Logger::LOG_ERROR, "no decent framebuffer config found");
        exit(EXIT_FAILURE);
    }
    
    Logger::getInstance().log(Logger::LOG_INFO, "found %d framebuffer configs", fbConfigsCount);
    
    GLXFBConfig fbConfig;
    if (Extention::isSupported(this->display, "GLX_ARB_multisample")) {
        Logger::getInstance().log(Logger::LOG_INFO, "GLX_ARB_multisample supported, looking for a decent framebuffer config");
        
        int samples, sampleBuffers, id, samplesBest = 0, sampleBuffersBest = 0, index = 0;
        for (int i = 0; i < fbConfigsCount; i++) {
            glXGetFBConfigAttrib(this->display, fbConfigs[i], GLX_FBCONFIG_ID, &id);
            glXGetFBConfigAttrib(this->display, fbConfigs[i], GLX_SAMPLES, &samples);
            glXGetFBConfigAttrib(this->display, fbConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
            
            Logger::getInstance().log(Logger::LOG_INFO, "  #%d 0x%04x: %d samples, %d sample buffes", i + 1, id, samples, sampleBuffers);

            if (samples > samplesBest || sampleBuffers > sampleBuffersBest) {
                samplesBest = samples;
                sampleBuffersBest = sampleBuffers;
                index = i;
            }
        }
        
        Logger::getInstance().log(Logger::LOG_INFO, "#%d has been choosen", index + 1);
        fbConfig = fbConfigs[index];
    } else {
        Logger::getInstance().log(Logger::LOG_INFO, "GLX_ARB_multisample unsupported, choosing first framebuffer config");
        fbConfig = fbConfigs[0];
    }
    XFree(fbConfigs);

    XVisualInfo *visualInfo = glXGetVisualFromFBConfig(this->display, fbConfig);
    if (!visualInfo) {
        Logger::getInstance().log(Logger::LOG_ERROR, "cannot choose visual");
        exit(EXIT_FAILURE);    
    }

    this->colormap = XCreateColormap(this->display, rootWindow, visualInfo->visual, AllocNone);

    XSetWindowAttributes windowAttributes;
    windowAttributes.colormap = this->colormap;
    windowAttributes.event_mask = PointerMotionMask | KeyPressMask | KeyReleaseMask |
                                  ButtonPressMask | ButtonReleaseMask;
    this->window = XCreateWindow(this->display, rootWindow, 0, 0, this->width, this->height, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask,
            &windowAttributes);
    XFree(visualInfo);
    XStoreName(this->display, this->window, this->caption.c_str());
    
    /* This actually registers WM_DELETE_WINDOW so we can intercept it */
    this->wmDeleteWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->display, this->window, &this->wmDeleteWindow, 1);
    
    this->wmCaptureMouse = XInternAtom(this->display, "WM_CAPTURE_MOUSE", False);
    
    XMapRaised(this->display, this->window);

//    int contextAttributes[] = {
//        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
//        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
//        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
//        None
//    };

//    TODO: check GL_ARB_pixel_buffer_object extension
//    this->glxContext = glXCreateContextAttribsARB(this->display, fbConfig,
//            NULL, True, contextAttributes);
    this->glxContext = glXCreateContext(this->display, visualInfo, 0, GL_TRUE);
    if (!this->glxContext) {
        XDestroyWindow(this->display, this->window);
        XFreeColormap(this->display, this->colormap);
        XCloseDisplay(this->display);

        Logger::getInstance().log(Logger::LOG_ERROR, "cannot create OpenGL 3.3 context");
        exit(EXIT_FAILURE);
    }

    glXMakeCurrent(this->display, this->window, this->glxContext);
}

void OpenGLWindow::destroyWindow() {
    glXMakeCurrent(this->display, None, None);
    glXDestroyContext(this->display, this->glxContext);
    XDestroyWindow(this->display, this->window);
    XFreeColormap(this->display, this->colormap);
    XCloseDisplay(this->display);
}
