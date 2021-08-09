#ifndef MAZE_CAMERA_H
#define MAZE_CAMERA_H

#include <glad/types.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class ShaderProgram;

class Camera {
private:
    int bindingPoint;
    GLuint vbo;
    glm::mat4x4 perspective;
    glm::mat4x4 view;
    glm::vec3 pos;
    glm::vec3 eyeDirection;
    glm::vec3 center;

    void move(glm::vec3 vec);

public:
    explicit Camera(float aspect);

    Camera(int width, int height);

    ~Camera();

    void updateShaderUniformLayout();

    void forward(float d);

    void left(float d);

    void right(float d);

    void back(float d);

    void up(float d);

    void down(float d);

    void rotate(float dx, float dy);

    void bindUniformLayout(ShaderProgram &program, const char *name);
};


#endif //MAZE_CAMERA_H
