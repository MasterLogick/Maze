#include "Shader.h"
#include <glad/gl.h>

Shader::Shader(GLenum type) {
    shader = glCreateShader(type);
}

Shader::~Shader() {
    glDeleteShader(shader);
}

int Shader::loadShader(char *code) {
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success;
}

void Shader::getInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    glGetShaderInfoLog(shader, bufSize, length, infoLog);
}

GLuint Shader::getID() {
    return shader;
}
