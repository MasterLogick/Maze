#ifndef MAZE_SHADERPROGRAM_H
#define MAZE_SHADERPROGRAM_H

#include <glad/types.h>
#include <map>
#include <string>

#define LOG_BUFFER_LENGTH 8192

class Shader;

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

    void bindUniformBlock(int bindingPoint, const GLchar *name);

    void uniform(const GLchar *name, float val);

    void uniform(const GLchar *name, int val);

    void uniform3(const GLchar *name, float *val);

    void uniform4(const GLchar *name, float *val);
};


#endif //MAZE_SHADERPROGRAM_H
