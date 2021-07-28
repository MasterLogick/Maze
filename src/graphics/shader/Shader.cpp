#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"

Shader::Shader(GLenum type) {
    shader = glCreateShader(type);
}

Shader::~Shader() {
    glDeleteShader(shader);
}

int Shader::loadShader(char *path) {
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_WAS_NOT_SUCCESSFULLY_READ" << std::endl << e.what() << std::endl;
    }
    const char *ccode = code.c_str();
    glShaderSource(shader, 1, &ccode, nullptr);
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
