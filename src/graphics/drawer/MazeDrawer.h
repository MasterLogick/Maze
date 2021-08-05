#ifndef MAZE_MAZEDRAWER_H
#define MAZE_MAZEDRAWER_H

#include <glad/types.h>
#include "../../core/maze_generator/MazeGenerator.h"

class MazeDrawer {
private:
    MazeGenerator &mazeGenerator;
    GLuint vao;
    GLuint vbo;
    int width;
    int height;
    int screenWidth;
    int screenHeight;
    float *mappedVertexes;
    GLsizeiptr vboLength;
public:
    explicit MazeDrawer(MazeGenerator &mazeGenerator, int screenWidth, int screenHeight);

    ~MazeDrawer();

    void drawMaze();

    void drawPlayer(Point p);
};


#endif //MAZE_MAZEDRAWER_H
