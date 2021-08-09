#include "ShaderProgram.h"
#include <glad/gl.h>
#include "Shader.h"

ShaderProgram::ShaderProgram() : attribLocations(), uniformLocations() {
    program = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(program);
}

void ShaderProgram::attachProgram(Shader &shader) {
    glAttachShader(program, shader.getID());
}

GLint ShaderProgram::link() {
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    return status;
}

void ShaderProgram::getInfoLog(GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void ShaderProgram::bind() {
    glUseProgram(program);
}

GLuint ShaderProgram::getAttribLocation(const GLchar *name) {
    if (attribLocations.find(name) == attribLocations.end())
        attribLocations[name] = glGetAttribLocation(program, name);
    return attribLocations[name];
}

void ShaderProgram::bindUniformBlock(int bindingPoint, const GLchar *name) {
    GLuint index = glGetUniformBlockIndex(program, name);
    glUniformBlockBinding(program, index, bindingPoint);
}

void ShaderProgram::uniform(const GLchar *name, float val) {
    if (uniformLocations.find(name) == uniformLocations.end())
        uniformLocations[name] = glGetUniformLocation(program, name);
    glUniform1f(uniformLocations[name], val);
}

void ShaderProgram::uniform(const GLchar *name, int val) {
    if (uniformLocations.find(name) == uniformLocations.end())
        uniformLocations[name] = glGetUniformLocation(program, name);
    glUniform1i(uniformLocations[name], val);
}

void ShaderProgram::uniform3(const GLchar *name, float *val) {
    if (uniformLocations.find(name) == uniformLocations.end())
        uniformLocations[name] = glGetUniformLocation(program, name);
    glUniform3fv(uniformLocations[name], 1, val);
}

void ShaderProgram::uniform4(const GLchar *name, float *val) {
    if (uniformLocations.find(name) == uniformLocations.end())
        uniformLocations[name] = glGetUniformLocation(program, name);
    glUniform4fv(uniformLocations[name], 1, val);
}
