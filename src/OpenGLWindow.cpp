#define GLX_GLXEXT_PROTOTYPES
#include <sys/time.h>
#include <memory.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>

#include "OpenGLWindow.h"
#include "Logger.h"
#include "Extention.h"
#include "configuration.h"

OpenGLWindow::OpenGLWindow() {
    this->width = 640;
    this->height = 480;
    this->toggleFullScreenRequest = false;
    
    this->initialize();
}

OpenGLWindow::OpenGLWindow(int width, int height, const std::string& caption, bool fullScreen):
        caption(caption) {
    this->width = width;
    this->height = height;
    this->toggleFullScreenRequest = fullScreen;

    this->initialize();
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
        if (this->terminateRequest) {
            if (this->fullScreen) {
                this->toggleFullScreen();
            // toggleFullScreenRequest == false means fullscreen mode has never been requested or
            // we have already switched back.
            // nativeResolutuionEnabled == true means we shouldn't wait for RRScreenChangeNotify
            // norification as screen geometry hasn't really changed.
            } else if (!this->toggleFullScreenRequest || this->nativeResolutuionEnabled) {
                break;
            }
        }

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

                default:
                    if (event.type - this->xrandrEventBase == RRScreenChangeNotify) {
                        // See http://www.x.org/archive/X11R7.5/doc/man/man3/Xrandr.3.html#sect4
                        XRRUpdateConfiguration(&event);

                        if (this->terminateRequest && !this->fullScreen) {
                            this->running = false;
                        }
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

void OpenGLWindow::initialize() {
    this->fullScreen = false;

    this->captured = false;
    this->running = true;
    this->msaaEnabled = false;
    this->nativeResolutuionEnabled = false;
    this->terminateRequest = false;

    this->xrandrEventBase = 0;
    this->frameTime = 0.0f;
    this->resolutionID = 0;
}

void OpenGLWindow::initWindow() {
    this->display = XOpenDisplay(NULL);
    if (this->display == NULL) {
        Logger::getInstance().log(Logger::LOG_ERROR, "cannot open display");
        exit(EXIT_FAILURE);
    }

    int screen = XDefaultScreen(this->display);
    this->rootWindow = XRootWindow(this->display, screen);

    if (!this->checkSupportedResolutions() || !this->checkFrameBufferConfigs()) {
        XCloseDisplay(this->display);
        exit(EXIT_FAILURE);
    }

    XWindowAttributes rootWindowAttributes;
    XGetWindowAttributes(this->display, this->rootWindow, &rootWindowAttributes);

    if (rootWindowAttributes.width == this->width && rootWindowAttributes.height == this->height) {
        this->nativeResolutuionEnabled = true;
    }

    this->colormap = XCreateColormap(this->display, this->rootWindow, this->visualInfo->visual,
            AllocNone);

    XSetWindowAttributes windowAttributes;
    windowAttributes.colormap = this->colormap;
    windowAttributes.event_mask = PointerMotionMask | KeyPressMask | KeyReleaseMask |
                                  ButtonPressMask | ButtonReleaseMask;
    this->window = XCreateWindow(this->display, this->rootWindow, 0, 0, this->width, this->height, 0,
            this->visualInfo->depth, InputOutput, this->visualInfo->visual,
            CWColormap | CWEventMask, &windowAttributes);

    XStoreName(this->display, this->window, this->caption.c_str());
    XMapRaised(this->display, this->window);
    XFree(this->visualInfo);
    
    /* This actually registers WM_DELETE_WINDOW so we can intercept it */
    this->wmDeleteWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->display, this->window, &this->wmDeleteWindow, 1);
    
    this->wmCaptureMouse = XInternAtom(this->display, "WM_CAPTURE_MOUSE", False);
    this->wmState = XInternAtom(this->display, "_NET_WM_STATE", False);
    this->wmFullScreen = XInternAtom(this->display, "_NET_WM_STATE_FULLSCREEN", False);
    
    const int contextAttributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None
    };

    if (!Extention::isSupported(this->display, "GLX_ARB_create_context")) {
        Logger::getInstance().log(Logger::LOG_ERROR, "GLX 1.4 or higher required");

        XDestroyWindow(this->display, this->window);
        XFreeColormap(this->display, this->colormap);
        XCloseDisplay(this->display);
        exit(EXIT_FAILURE);
    }

    this->glxContext = glXCreateContextAttribsARB(this->display, this->fbConfig, NULL, True,
            contextAttributes);

    if (this->glxContext == NULL) {
        Logger::getInstance().log(Logger::LOG_ERROR, "cannot create OpenGL 3.3 context");
        
        XDestroyWindow(this->display, this->window);
        XFreeColormap(this->display, this->colormap);
        XCloseDisplay(this->display);
        exit(EXIT_FAILURE);
    }

    if (!glXIsDirect(this->display, this->glxContext)) {
        Logger::getInstance().log(Logger::LOG_ERROR, "direct rendering disabled");
        
        glXDestroyContext(this->display, this->glxContext);
        XDestroyWindow(this->display, this->window);
        XFreeColormap(this->display, this->colormap);
        XCloseDisplay(this->display);
        exit(EXIT_FAILURE);  
    }
    
    glXMakeCurrent(this->display, this->window, this->glxContext);

    if (this->toggleFullScreenRequest) {
        Logger::getInstance().log(Logger::LOG_INFO, "Switching into fullscreen mode");
        this->toggleFullScreen();
    }

    this->initVSync();
    this->initOpenGL();
}

void OpenGLWindow::destroyWindow() {
    glXMakeCurrent(this->display, None, None);
    glXDestroyContext(this->display, this->glxContext);
    XDestroyWindow(this->display, this->window);
    XFreeColormap(this->display, this->colormap);
    XCloseDisplay(this->display);
}

bool OpenGLWindow::checkSupportedResolutions() {
    XRRScreenConfiguration *screenConfig = XRRGetScreenInfo(this->display, this->rootWindow);
    if (screenConfig == NULL) {
        return false;
    }

    int configSizesNumber;
    XRRScreenSize *configSizes = XRRConfigSizes(screenConfig, &configSizesNumber);
    if (configSizes == NULL) {
        return false;
    }

    // This will give us all resolutions available
    std::map<std::pair<int, int>, SizeID> availableResolutions;
    for (int i = 0; i < configSizesNumber; i++) {
        std::pair<int, int> resolution(configSizes[i].width, configSizes[i].height);
        availableResolutions.insert(std::make_pair(resolution, i));
    }

    XRRFreeScreenConfigInfo(screenConfig);

    // This will give us all resolutions available for cloned outputs.
    // Its tricky as one output can support resolutions that other doesn't.
    XRRScreenResources *screenResources = XRRGetScreenResourcesCurrent(this->display,
            this->rootWindow);

    Logger::getInstance().log(Logger::LOG_INFO, "Requested resolution: %d x %d",
            this->width, this->height);
    Logger::getInstance().log(Logger::LOG_INFO, "Looking for supported screen resolutions");

    std::set<std::pair<int, int>> uniqueResolutions;
    for (int i = 0; i < screenResources->nmode; i++) {
        std::pair<int, int> resolution(screenResources->modes[i].width,
                                       screenResources->modes[i].height);
        uniqueResolutions.insert(resolution);
    }

    // Sorted output, handy for multimonitor configuration
    int resolutionIndex = 0;
    std::set<std::pair<int, int>>::const_reverse_iterator uniqueResolution =
            uniqueResolutions.rbegin();
    for ( ; uniqueResolution != uniqueResolutions.rend(); uniqueResolution++) {
        Logger::getInstance().log(Logger::LOG_INFO, "  #%d %d x %d", resolutionIndex++,
                uniqueResolution->first, uniqueResolution->second);
    }

    // Check if requested resolution (this->width, this->height) supported
    std::map<std::pair<int, int>, SizeID>::const_iterator resolution =
            availableResolutions.find(std::make_pair(this->width, this->height));
    if (resolution != availableResolutions.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "Resolution %d x %d supported",
                this->width, this->height);
        this->resolutionID = resolution->second;
    } else {
        Logger::getInstance().log(Logger::LOG_WARNING, "Resolution %d x %d not supported",
                this->width, this->height);
        this->width = availableResolutions.begin()->first.first;
        this->height = availableResolutions.begin()->first.second;

        Logger::getInstance().log(Logger::LOG_WARNING, "  defaulting to %d x %d",
                this->width, this->height);
        this->resolutionID = availableResolutions.begin()->second;
    }

    XRRFreeScreenResources(screenResources);

    int xrandrErrorBase;
    XRRQueryExtension(this->display, &this->xrandrEventBase, &xrandrErrorBase);
    XRRSelectInput(this->display, this->rootWindow, RRScreenChangeNotifyMask);

    return true;
}

bool OpenGLWindow::checkFrameBufferConfigs() {
    const int fbAttributes[] = {
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

    int fbConfigsCount;
    GLXFBConfig *fbConfigs = glXChooseFBConfig(this->display, DefaultScreen(this->display),
            fbAttributes, &fbConfigsCount);
    if (fbConfigs == NULL) {
        Logger::getInstance().log(Logger::LOG_ERROR, "No framebuffer configs found");
        return false;
    }

    Logger::getInstance().log(Logger::LOG_INFO, "Requested MSAA level: %dx", MSAA);
    Logger::getInstance().log(Logger::LOG_INFO, "Looking through available framebuffer configs");

    int samples, sampleBuffers, id, fbIndex = 0;
    std::map<std::pair<int, int>, std::pair<GLXFBConfig, XVisualInfo*>> uniqueFBConfigs;
    for (int i = 0; i < fbConfigsCount; i++) {
        this->fbConfig = fbConfigs[i];
        this->visualInfo = glXGetVisualFromFBConfig(this->display, this->fbConfig);

        // Visuals with screen depth == 32 produce XCreateWindow() error:
        // X Error of failed request:  BadMatch (invalid parameter attributes)
        if (this->visualInfo->depth > 24) {
            XFree(this->visualInfo);
            continue;
        }

        glXGetFBConfigAttrib(this->display, this->fbConfig, GLX_FBCONFIG_ID, &id);
        glXGetFBConfigAttrib(this->display, this->fbConfig, GLX_SAMPLES, &samples);
        glXGetFBConfigAttrib(this->display, this->fbConfig, GLX_SAMPLE_BUFFERS, &sampleBuffers);

        std::pair<int, int> msaaMode(samples, sampleBuffers);
        std::pair<GLXFBConfig, XVisualInfo*> msaaModeParameters(this->fbConfig, this->visualInfo);
        if (uniqueFBConfigs.insert(std::make_pair(msaaMode, msaaModeParameters)).second) {
            Logger::getInstance().log(Logger::LOG_INFO, "  #%d 0x%04x: %d samples, %d sample buffers",
                    fbIndex++, id, samples, sampleBuffers);
        }
    }

    XFree(fbConfigs);

    // In a parrallel universe...
    if (uniqueFBConfigs.size() == 0) {
        Logger::getInstance().log(Logger::LOG_ERROR, "No framebuffer config choosen");
        return false;
    }

    std::map<std::pair<int, int>, std::pair<GLXFBConfig, XVisualInfo*>>::const_iterator mode =
            uniqueFBConfigs.find(std::make_pair(MSAA, (MSAA > 0 ? 1 : 0)));
    // You must be kidding me...
    if (mode == uniqueFBConfigs.end() && MSAA == 0) {
        Logger::getInstance().log(Logger::LOG_ERROR, "No framebuffer config choosen");
        return false;
    }

    if (mode != uniqueFBConfigs.end()) {
        Logger::getInstance().log(Logger::LOG_INFO, "MSAA level %dx supported", MSAA);
        this->msaaEnabled = true;
    } else {
        Logger::getInstance().log(Logger::LOG_INFO, "MSAA level %dx not supported", MSAA);
        Logger::getInstance().log(Logger::LOG_INFO, "  falling back to 0x MSAA level");
        mode = uniqueFBConfigs.find(std::make_pair(0, 0));
    }

    this->fbConfig = mode->second.first;
    this->visualInfo = mode->second.second;

    for (mode = uniqueFBConfigs.begin(); mode != uniqueFBConfigs.end(); mode++) {
        if (mode->second.second != this->visualInfo) {
            XFree(mode->second.second);
        }
    }

    return true;
}

void OpenGLWindow::initVSync() {
    bool tearControlSupported = false, swapControlSupported = false;

    if (Extention::isSupported(this->display, "GLX_EXT_swap_control")) {
        swapControlSupported = true;
        Logger::getInstance().log(Logger::LOG_INFO, "GLX_EXT_swap_control supported");
    }

    if (Extention::isSupported(this->display, "GLX_EXT_swap_control_tear")) {
        tearControlSupported = true;
        Logger::getInstance().log(Logger::LOG_INFO, "GLX_EXT_swap_control_tear supported");
    }

    if (!tearControlSupported && !swapControlSupported) {
        Logger::getInstance().log(Logger::LOG_INFO, "VSYNC management unavailable");
    } else {
        if (VSYNC > 1 && tearControlSupported) {
            glXSwapIntervalEXT(this->display, this->window, -1);
            Logger::getInstance().log(Logger::LOG_INFO, "  adaptive VSYNC enabled");
        } else if (VSYNC > 0 && swapControlSupported) {
            glXSwapIntervalEXT(this->display, this->window, 1);
            Logger::getInstance().log(Logger::LOG_INFO, "  VSYNC enabled");
        } else if (swapControlSupported) {
            glXSwapIntervalEXT(this->display, this->window, 0);
            Logger::getInstance().log(Logger::LOG_INFO, "  VSYNC disabled");
        }
    }
}

void OpenGLWindow::initOpenGL() {
    Logger::getInstance().log(Logger::LOG_INFO, "OpenGL renderer: %s", glGetString(GL_RENDERER));
    Logger::getInstance().log(Logger::LOG_INFO, "OpenGL vendor: %s", glGetString(GL_VENDOR));

    glViewport(0, 0, (GLsizei)this->width, (GLsizei)this->height);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // NOTE: For FBO rendering with MSAA one should use GL_TEXTURE_2D_MULTISAMPLE texture targets
    if (this->msaaEnabled) {
        glEnable(GL_MULTISAMPLE);
    }

    if (Extention::isSupported("GL_ARB_seamless_cube_map")) {
        Logger::getInstance().log(Logger::LOG_INFO, "GL_ARB_seamless_cube_map supported");

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        Logger::getInstance().log(Logger::LOG_INFO, "  all cubemap textures will be seamless");
    }
}

void OpenGLWindow::toggleFullScreen() {
    Rotation rotation;
    XRRScreenConfiguration *screenConfig = XRRGetScreenInfo(this->display, this->rootWindow);

    // See http://standards.freedesktop.org/wm-spec/wm-spec-1.3.html#id2731936
    XClientMessageEvent toggleFullScreenEvent;
    toggleFullScreenEvent.window = this->window;
    toggleFullScreenEvent.message_type = this->wmState;
    toggleFullScreenEvent.type = ClientMessage;
    toggleFullScreenEvent.format = 32;
    toggleFullScreenEvent.data.l[0] = 2;
    toggleFullScreenEvent.data.l[1] = this->wmFullScreen;
    toggleFullScreenEvent.data.l[2] = 0;
    toggleFullScreenEvent.data.l[3] = 1;
    toggleFullScreenEvent.data.l[4] = 0;

    long eventMask = SubstructureNotifyMask | SubstructureRedirectMask;

    if (!this->nativeResolutuionEnabled) {
        if (!this->fullScreen) {
            this->oldResolutionID = XRRConfigCurrentConfiguration(screenConfig, &rotation);
            XRRSetScreenConfig(this->display, screenConfig, this->rootWindow, this->resolutionID,
                    rotation, CurrentTime);
        } else {
            XRRConfigCurrentConfiguration(screenConfig, &rotation);
            XRRSetScreenConfig(this->display, screenConfig, this->rootWindow, this->oldResolutionID,
                    rotation, CurrentTime);
        }
    }

    XSendEvent(this->display, this->window, True, eventMask, (XEvent*)&toggleFullScreenEvent);

    XRRFreeScreenConfigInfo(screenConfig);
    this->fullScreen = !this->fullScreen;
}
