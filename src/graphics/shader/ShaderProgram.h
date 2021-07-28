#ifndef MAZE_SHADERPROGRAM_H
#define MAZE_SHADERPROGRAM_H

#include <glad/gl.h>
#include <map>
#include <string>
#include "Shader.h"

#define LOG_BUFFER_LENGTH 8192

class ShaderProgram {
private:
    GLuint program;
    std::map<std::string, int> attribLocations;
    std::map<std::string, int> uniformLocations;
public:
    ShaderProgram();

    ~ShaderProgram();

    void attachProgram(Shader &shader);

    GLint link();

    void getInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog);

    void bind();

    GLuint getAttribLocation(const GLchar *name);

    void bindUniformBlock(int bindingPoint, char *name);

    void uniform(const char *name, float val);

    void uniform(const char *name, int val);

    void uniform3(const char *name, float *val);

    void uniform4(const char *name, float *val);
};


#endif //MAZE_SHADERPROGRAM_H
