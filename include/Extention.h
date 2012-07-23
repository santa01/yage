#ifndef EXTENTION_H
#define	EXTENTION_H

#include <X11/Xlib.h>
#include <string>

class Extention {
    public:
        static bool isSupported(const std::string& extention);
        static bool isSupported(Display *dpy, const std::string& extention);
};

#endif	/* EXTENTION_H */
