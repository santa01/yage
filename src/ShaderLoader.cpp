#include <fstream>
#include <string>
#include <vector>

#include "ShaderLoader.h"
#include "Logger.h"

GLuint ShaderLoader::createShader(const std::string& name) {
    const std::string ext = name.substr(name.length() - 3);
    
    if (ext.compare(".fs") == 0) return ShaderLoader::shaderFromFile(name, GL_FRAGMENT_SHADER);
    if (ext.compare(".gs") == 0) return ShaderLoader::shaderFromFile(name, GL_GEOMETRY_SHADER);
    if (ext.compare(".vs") == 0) return ShaderLoader::shaderFromFile(name, GL_VERTEX_SHADER);

    Logger::getInstance().log(Logger::LOG_WARNING, "Cannot determine type of %s", name.c_str());
    return 0;
}

GLuint ShaderLoader::createProgram(const std::vector<std::string>& shaders) {
    GLuint program = glCreateProgram();
    
    std::vector<std::string>::const_iterator shader;
    for (shader = shaders.begin(); shader != shaders.end(); shader++) {
        GLuint shaderObject = ShaderLoader::createShader(*shader);
        glAttachShader(program, shaderObject);
        glDeleteShader(shaderObject);
    }
    
    glLinkProgram(program);

    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        GLchar* infoLog = new GLchar[infoLogLength];
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);

        GLint linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        int level = (linkStatus == GL_FALSE) ? Logger::LOG_ERROR : Logger::LOG_INFO;
        Logger::getInstance().log(level, "  %s", infoLog);

        delete[] infoLog;
    }

    return program;
}

GLuint ShaderLoader::shaderFromFile(const std::string& name, GLenum type) {
    std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
    if (!file.good()) {
        Logger::getInstance().log(Logger::LOG_ERROR, "Cannot open file %s", name.c_str());
        return 0;
    }

    file.seekg(0, std::ios::end);
    int sourceLength = file.tellg();
    GLchar* shaderSource = new GLchar[sourceLength + 1];

    file.seekg(0, std::ios::beg);
    file.read(shaderSource, sourceLength);
    shaderSource[sourceLength] = 0;

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&shaderSource, NULL);
    glCompileShader(shader);

    delete[] shaderSource;

    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if (infoLogLength > 0) {
        GLchar* infoLog = new GLchar[infoLogLength];
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

        GLint compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        int level = (compileStatus == GL_FALSE) ? Logger::LOG_ERROR : Logger::LOG_INFO;
        Logger::getInstance().log(level, "%s: %s", name.c_str(), infoLog);

        delete[] infoLog;
    }

    return shader;
}
