#ifndef SHADERLOADER_H
#define	SHADERLOADER_H

#include <global.h>
#include <string>
#include <vector>

class ShaderLoader {
    public:
        static GLuint createShader(const std::string& name);
        static GLuint createProgram(const std::vector<GLuint>& shaders);

    private:
        static GLuint shaderFromFile(const std::string& name, GLenum type);
};

#endif	/* SHADERLOADER_H */
