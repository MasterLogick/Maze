#ifndef MAZE_SHADER_H
#define MAZE_SHADER_H

#include <string>
#include <map>
#include <glad/gl.h>


class Shader {
private:
    GLuint program;
    std::map<std::string, int> *uniforms;
    int id;

    void bindUniform(int bindingPoint, char *name);

public:
    static Shader *triangleShader;
    static Shader *postProcessorShader;
    static Shader *fontShader;
    static Shader *spriteDrawingShader;
    static Shader *videoDrawingShader;

    Shader(GLuint id, std::map<std::string, int> *uniforms);

    void bind();

    GLuint getAttribLocation(const GLchar *name);

    void uniform(const char *name, float val);

    void uniform(const char *name, int val);

    static void initialise();

    void uniform3(const char name[6], float *val);

    void uniform4(const char name[6], float *val);
};


#endif //MAZE_SHADER_H
