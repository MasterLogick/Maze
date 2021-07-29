#ifndef MAZE_MSAAPOSTPROCESSOR_H
#define MAZE_MSAAPOSTPROCESSOR_H

#include <glad/types.h>

class MSAAPostProcessor {
private:
    int width;
    int height;
    GLuint vao;
    GLuint vbo;
    GLuint postProcessingFramebuffer;
    GLuint postProcessingTexture;
    GLuint multiSampledFramebuffer;
    GLuint multiSampledTexture;
    GLuint multiSampledRenderbuffer;

public:

    explicit MSAAPostProcessor(int level, int width, int height);

    void prepareContext();

    void postProcess(GLuint framebuffer);
};


#endif //MAZE_MSAAPOSTPROCESSOR_H
