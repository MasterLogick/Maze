#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "graphics/shader/Shader.h"
#include "graphics/shader/ShaderProgram.h"
#include "graphics/post_proccessing/MSAAPostProcessor.h"
#include "graphics/shader/Shaders.h"
#include "core/maze_generator/MazeGenerator.h"
#include "graphics/drawer/MazeDrawer.h"
#include "graphics/camera/Camera.h"
#include "graphics/fps_counter/FPSCounter.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define SCREEN_INFO_BINDING_POINT 0

bool shouldClose = false;
Camera *cam;

void shouldCloseCallback(GLFWwindow *window) {
    shouldClose = true;
    std::cout << "lol" << std::endl;
}

double xpos, ypos;

void processCallbacks(GLFWwindow *window) {
    glfwPollEvents();
    double dxpos, dypos;
    glfwGetCursorPos(window, &dxpos, &dypos);
    cam->rotate(xpos - dxpos, ypos - dypos);
    xpos = dxpos;
    ypos = dypos;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        shouldClose = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->forward(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->left(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->back(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->right(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam->up(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cam->down(1.0f);
    }
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
    if (!glfwInit()) {
        std::cout << "Can't initialise GLFW" << std::endl;
        return -1;
    }
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //OS X compilation fix
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Maze", nullptr, nullptr);
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
    glfwSwapInterval(0);
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(message_callback, nullptr);
#endif
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    initShaders();

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT);
    camera.bindUniformLayout(*basic_Shader, "Camera");
    camera.bindUniformLayout(*border_Shader, "Camera");
    cam = &camera;

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

    MazeGenerator generator(1, 1);
    MazeDrawer drawer(generator, WINDOW_WIDTH, WINDOW_HEIGHT);
    while (!generator.isComplete()) {
        generator.nextStep();
    }

    FPSCounter counter(60);

    while (!shouldClose) {
        processCallbacks(window);
        camera.updateShaderUniformLayout();
        processor.prepareContext();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        drawer.drawMaze();
        processor.postProcess(0);
        glfwSwapBuffers(window);
        counter.countFPSAndSleep();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
