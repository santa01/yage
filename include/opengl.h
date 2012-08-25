#ifndef OPENGL_H
#define OPENGL_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include "GL/glext.h"

#if defined(_WIN32)
#  include "GL/wglext.h"
#else
#  include "GL/glxext.h"
#endif

#endif /* OPENGL_H */
