#ifndef MAZE_SHADERLOADER_H
#define MAZE_SHADERLOADER_H

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"
#include "platformDependentDefines.h"

#define ERROR_BUFFER_SIZE 8192

class ShaderLoader {
private:
    static GLuint createShader(const char *code, GLenum type);

    static GLuint linkShaderProgram(GLint vertexShader, GLint fragmentShader, GLuint geometryShader = 0);

    static std::map<std::string, int> *getUniformsLocations(int program, char **uniforms, int count);

public:

    static Shader *loadShader(char *name, char **uniforms, int count);
};


#endif //MAZE_SHADERLOADER_H
