#include <string>

#include "Extention.h"

bool Extention::isSupported(const std::string& extention) {
    GLint extensions;

    glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
    for (int i = 0; i < extensions; i++) {
        if (extention.compare((char*)glGetStringi(GL_EXTENSIONS, i)) == 0) {
            return true;
        }
    }
    
    return false;
}

bool Extention::isSupported(Display *dpy, const std::string& extention) {
    std::string extensions(glXQueryExtensionsString(dpy, DefaultScreen(dpy)));
    return (extensions.find(extention) != std::string::npos);
}
