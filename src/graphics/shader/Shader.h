#ifndef MAZE_SHADER_H
#define MAZE_SHADER_H

#include <glad/gl.h>

class Shader {
private:
    GLuint shader;

public:
    explicit Shader(GLenum type);

    ~Shader();

    int loadShader(char *path);

    void getInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog);

    GLuint getID();
};


#endif //MAZE_SHADER_H
