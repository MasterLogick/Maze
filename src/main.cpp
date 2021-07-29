#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/shader/Shader.h"
#include "graphics/shader/ShaderProgram.h"
#include "graphics/post_proccessing/MSAAPostProcessor.h"
#include "graphics/shader/Shaders.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define SCREEN_INFO_BINDING_POINT 0

bool shouldClose = false;

void shouldCloseCallback(GLFWwindow *window) {
    shouldClose = true;
    std::cout << "lol" << std::endl;
}

void error_callback(int error, const char *description) {
    std::cout << "Error" << error << " " << description << std::endl;
}

#ifndef NDEBUG

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                 const GLchar *message,
                                 const void *userParam) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    if (source == GL_DEBUG_SOURCE_SHADER_COMPILER && type == GL_DEBUG_TYPE_OTHER &&
        severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << "---------------" << std::endl;
}

#endif

int main() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::cout << "Can't initialise GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwDefaultWindowHints();
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //OS X compilation fix
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze", NULL, NULL);
    if (!window) {
        std::cout << "Can't create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Can't load GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glfwSetWindowCloseCallback(window, shouldCloseCallback);
    glfwSwapInterval(1);
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(message_callback, nullptr);
#endif
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    initShaders();

    glm::mat4x4 ortho = glm::ortho<float>(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    GLuint blockVBO;
    glCreateBuffers(1, &blockVBO);
    glNamedBufferData(blockVBO, sizeof(glm::mat4x4) + 4 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glNamedBufferSubData(blockVBO, 0L, sizeof(glm::mat4x4), glm::value_ptr(ortho));
    float width = WINDOW_WIDTH;
    float height = WINDOW_HEIGHT;
    glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4), sizeof(float), &width);
    glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4) + sizeof(float), sizeof(float), &height);
    glNamedBufferSubData(blockVBO, sizeof(glm::mat4x4) + 2 * sizeof(float), 2 * sizeof(float), nullptr);
    glBindBufferRange(GL_UNIFORM_BUFFER, SCREEN_INFO_BINDING_POINT, blockVBO, 0,
                      sizeof(glm::mat4x4) + 2 * sizeof(float));

    basic_Shader->bindUniformBlock(SCREEN_INFO_BINDING_POINT, const_cast<char *>("Screen"));

    GLuint vao;
    GLuint vbo;
    float positions[]{0, 0, WINDOW_WIDTH, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT};

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, 3 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    GLuint attribLoc = basic_Shader->getAttribLocation("localPos");
    glEnableVertexArrayAttrib(vao, attribLoc);
    glVertexArrayAttribFormat(vao, attribLoc, 2, GL_FLOAT, false, 0);
    glVertexArrayVertexBuffer(vao, attribLoc, vbo, 0, 2 * sizeof(float));

    MSAAPostProcessor processor(4, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!shouldClose) {
        processor.prepareContext();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        basic_Shader->bind();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        processor.postProcess(0);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Hello, World!" << std::endl;
    glfwTerminate();
    return 0;
}
