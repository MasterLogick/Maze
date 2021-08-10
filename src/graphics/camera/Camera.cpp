#include "Camera.h"
#include <atomic>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include "../shader/ShaderProgram.h"

std::atomic<int> BindingPoint(0);
glm::vec3 UP(0, 1, 0);

Camera::Camera(float aspect) {
    bindingPoint = BindingPoint;
    BindingPoint++;
    perspective = glm::perspective<float>(glm::radians(80.0f), aspect, 0.1f,
                                          1'000'000.0f);
    pos = glm::vec3(0);
    eyeDirection = glm::vec3(1, 0, 0);
    center = pos + eyeDirection;
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, 2 * sizeof(glm::mat4x4), nullptr, GL_STREAM_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, vbo);
}

Camera::Camera(int width, int height) : Camera(((float) width) / ((float) height)) {
}

Camera::~Camera() {
    glDeleteBuffers(1, &vbo);
}

void Camera::updateShaderUniformLayout() {
    view = glm::lookAt(pos, center, UP);
    glNamedBufferSubData(vbo, 0, sizeof(glm::mat4x4), glm::value_ptr(perspective));
    glNamedBufferSubData(vbo, sizeof(glm::mat4x4), sizeof(glm::mat4x4), glm::value_ptr(view));
}

void Camera::move(glm::vec3 vec) {
    pos += vec;
    center += vec;
}

void Camera::forward(float d) {
    glm::vec3 forwardDirection(eyeDirection.x, 0, eyeDirection.z);
    forwardDirection = d * glm::normalize(forwardDirection);
    move(forwardDirection);
}

void Camera::left(float d) {
    glm::vec3 leftDirection = d * glm::normalize(glm::cross(UP, eyeDirection));
    move(leftDirection);
}

void Camera::right(float d) {
    glm::vec3 rightDirection = d * glm::normalize(glm::cross(eyeDirection, UP));
    move(rightDirection);
}

void Camera::back(float d) {
    glm::vec3 backDirection(-eyeDirection.x, 0, -eyeDirection.z);
    backDirection = d * glm::normalize(backDirection);
    move(backDirection);
}

void Camera::up(float d) {
    move(d * UP);
}

void Camera::down(float d) {
    move(-d * UP);
}

void Camera::rotate(float dx, float dy) {
    eyeDirection = glm::vec3(glm::rotateNormalizedAxis(
            glm::rotateNormalizedAxis(glm::identity<glm::mat4x4>(), glm::radians(dx), glm::vec3(0, 1, 0)),
            glm::radians(dy), glm::normalize(glm::cross(UP, eyeDirection))) * glm::vec4(eyeDirection, 1.0f));
    center = pos + eyeDirection;
}

void Camera::bindUniformLayout(ShaderProgram &program, const char *name) {
    program.bindUniformBlock(bindingPoint, name);
}

