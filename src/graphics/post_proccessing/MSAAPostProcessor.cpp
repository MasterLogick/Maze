#include "MSAAPostProcessor.h"
#include "../shader/Shader.h"
#include <iostream>

static float verticesData[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
};

MSAAPostProcessor::MSAAPostProcessor(int level, int width, int height) : width(width), height(height) {
    //creating post processing framebuffer and texture
    glCreateFramebuffers(1, &postProcessingFramebuffer);
    glCreateTextures(GL_TEXTURE_2D, 1, &postProcessingTexture);
    //configuring post processing texture
    glTextureParameteri(postProcessingTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(postProcessingTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(postProcessingTexture, 1, GL_RGB8, width, height);
    //texture binding and checking
    glNamedFramebufferTexture(postProcessingFramebuffer, GL_COLOR_ATTACHMENT0, postProcessingTexture, 0);
    if (glCheckNamedFramebufferStatus(postProcessingFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "PostProcessor framebuffer creating error "
                  << glCheckNamedFramebufferStatus(postProcessingFramebuffer, GL_FRAMEBUFFER) << std::endl;
    }
    //creating multi sampled framebuffer and texture
    glCreateFramebuffers(1, &multiSampledFramebuffer);
    glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &multiSampledTexture);
    //configuring multi sampled texture
    glTextureStorage2DMultisample(multiSampledTexture, level, GL_RGBA8, width, height, GL_TRUE);
    glNamedFramebufferTexture(multiSampledFramebuffer, GL_COLOR_ATTACHMENT0, multiSampledTexture, 0);
    //creating renderbuffer
    glCreateRenderbuffers(1, &multiSampledRenderbuffer);
    //configuring and checking renderbuffer
    glNamedRenderbufferStorageMultisample(multiSampledRenderbuffer, level, GL_DEPTH24_STENCIL8, width, height);
    glNamedFramebufferRenderbuffer(multiSampledFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                   multiSampledRenderbuffer);
    if (glCheckNamedFramebufferStatus(multiSampledFramebuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "MultiSampledRenderbuffer framebuffer creating error "
                  << glCheckNamedFramebufferStatus(multiSampledFramebuffer, GL_FRAMEBUFFER) << std::endl;
    }
    //creating vertex buffers and mapping memory for future writes
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    Shader vert(GL_VERTEX_SHADER);
    if (!vert.loadShader("/home/user/CLionProjects/Maze/res/shaders/msaa/vertex.glsl")) {
        GLchar log[LOG_BUFFER_LENGTH];
        vert.getInfoLog(LOG_BUFFER_LENGTH, nullptr, log);
        std::cout << log << std::endl;
    }
    Shader frag(GL_FRAGMENT_SHADER);
    if (!frag.loadShader("/home/user/CLionProjects/Maze/res/shaders/msaa/fragment.glsl")) {
        GLchar log[LOG_BUFFER_LENGTH];
        frag.getInfoLog(LOG_BUFFER_LENGTH, nullptr, log);
        std::cout << log << std::endl;
    }
    shaderProgram.attachProgram(vert);
    shaderProgram.attachProgram(frag);
    shaderProgram.link();

    GLuint positionLocation = shaderProgram.getAttribLocation("position");
    GLuint texCordLocation = shaderProgram.getAttribLocation("texCord");

    glNamedBufferData(vbo, 4 * 2 * 2 * sizeof(float), verticesData, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(vao, positionLocation);
    glVertexArrayAttribFormat(vao, positionLocation, 2, GL_FLOAT, false, 0);
    glVertexArrayVertexBuffer(vao, positionLocation, vbo, 0, 2 * 2 * sizeof(float));

    glEnableVertexArrayAttrib(vao, texCordLocation);
    glVertexArrayAttribFormat(vao, texCordLocation, 2, GL_FLOAT, false, 0);
    glVertexArrayVertexBuffer(vao, texCordLocation, vbo, 2 * sizeof(float), 2 * 2 * sizeof(float));

    shaderProgram.bind();
    shaderProgram.uniform("screenTexture", 0);
}

void MSAAPostProcessor::prepareContext() {
    glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFramebuffer);
}

void MSAAPostProcessor::postProcess(GLuint framebuffer) {
    glBlitNamedFramebuffer(multiSampledFramebuffer, postProcessingFramebuffer, 0, 0, width, height, 0, 0, width, height,
                           GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    shaderProgram.bind();
    glBindTextureUnit(0, postProcessingTexture);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
}
