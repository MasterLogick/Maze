#include "MazeDrawer.h"
#include "../shader/Shaders.h"
#include "../shader/ShaderProgram.h"
#include <glad/gl.h>

#define PARTS 8.0f
#define ONE_PART 1.0f/PARTS
#define BIGGEST_PART (PARTS-1.0f)/PARTS

MazeDrawer::MazeDrawer(MazeGenerator &mazeGenerator, int screenWidth, int screenHeight) : mazeGenerator(mazeGenerator),
                                                                                          width(mazeGenerator.getWidth()),
                                                                                          height(mazeGenerator.getHeight()),
                                                                                          screenWidth(screenWidth),
                                                                                          screenHeight(screenHeight),
                                                                                          vboLength(
                                                                                                  (width * height * 4 +
                                                                                                   1) * 4 * 2 *
                                                                                                  sizeof(float)) {
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vboLength, nullptr,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT);
    GLuint attribLoc = basic_Shader->getAttribLocation("localPos");
    glEnableVertexArrayAttrib(vao, attribLoc);
    glVertexArrayAttribFormat(vao, attribLoc, 2, GL_FLOAT, false, 0);
    glVertexArrayVertexBuffer(vao, attribLoc, vbo, 0, 2 * sizeof(float));
    mappedVertexes = static_cast<float *>(glMapNamedBufferRange(vbo, 0, vboLength,
                                                                GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT |
                                                                GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));
}

MazeDrawer::~MazeDrawer() {
    glDeleteVertexArrays(1, &vao);
    glUnmapNamedBuffer(vbo);
    glDeleteBuffers(1, &vbo);
}

void MazeDrawer::drawMaze() {
    Cell **maze = mazeGenerator.getMaze();
    float cellWidth = ((float) screenWidth) / width;
    float cellHeight = ((float) screenHeight) / height;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Cell &cell = maze[i][j];
            float baseX = i * cellWidth;
            float baseY = j * cellHeight;
            if (cell.hasBorder(NORTH_SIDE)) {
                mappedVertexes[(i * height + j) * 32 + 0] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 1] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 2] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 3] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 4] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 5] = baseY + cellHeight * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 6] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 7] = baseY + cellHeight * BIGGEST_PART;
            } else {
                mappedVertexes[(i * height + j) * 32 + 0] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 1] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 2] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 3] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 4] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 5] = baseY + cellHeight * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 6] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 7] = baseY + cellHeight * BIGGEST_PART;
            }
            if (cell.hasBorder(WEST_SIDE)) {
                mappedVertexes[(i * height + j) * 32 + 8] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 9] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 10] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 11] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 12] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 13] = baseY + 0;
                mappedVertexes[(i * height + j) * 32 + 14] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 15] = baseY + 0;
            } else {
                mappedVertexes[(i * height + j) * 32 + 8] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 9] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 10] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 11] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 12] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 13] = baseY + cellHeight * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 14] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 15] = baseY + cellHeight * BIGGEST_PART;
            }
            if (cell.hasBorder(SOUTH_SIDE)) {
                mappedVertexes[(i * height + j) * 32 + 16] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 17] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 18] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 19] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 20] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 21] = baseY + 0;
                mappedVertexes[(i * height + j) * 32 + 22] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 23] = baseY + 0;
            } else {
                mappedVertexes[(i * height + j) * 32 + 16] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 17] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 18] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 19] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 20] = baseX + cellWidth * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 21] = baseY + 0;
                mappedVertexes[(i * height + j) * 32 + 22] = baseX + 0;
                mappedVertexes[(i * height + j) * 32 + 23] = baseY + 0;
            }
            if (cell.hasBorder(EAST_SIDE)) {
                mappedVertexes[(i * height + j) * 32 + 24] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 25] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 26] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 27] = baseY + cellHeight;
                mappedVertexes[(i * height + j) * 32 + 28] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 29] = baseY + 0;
                mappedVertexes[(i * height + j) * 32 + 30] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 31] = baseY + 0;
            } else {
                mappedVertexes[(i * height + j) * 32 + 24] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 25] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 26] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 27] = baseY + cellHeight * ONE_PART;
                mappedVertexes[(i * height + j) * 32 + 28] = baseX + cellWidth;
                mappedVertexes[(i * height + j) * 32 + 29] = baseY + 0;
                mappedVertexes[(i * height + j) * 32 + 30] = baseX + cellWidth * BIGGEST_PART;
                mappedVertexes[(i * height + j) * 32 + 31] = baseY + 0;
            }
        }
    }
    glFlushMappedNamedBufferRange(vbo, 0, vboLength - 4 * 2 * sizeof(float));
    basic_Shader->bind();
    glBindVertexArray(vao);
    for (int i = 0; i < width * height * 4; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
}

void MazeDrawer::drawPlayer(Point p) {
    float cellWidth = ((float) screenWidth) / width;
    float cellHeight = ((float) screenHeight) / height;
    float baseX = p.x * cellWidth;
    float baseY = p.y * cellHeight;
    mappedVertexes[width * height * 32 + 0] = baseX + cellWidth * BIGGEST_PART;
    mappedVertexes[width * height * 32 + 1] = baseY + cellHeight * BIGGEST_PART;
    mappedVertexes[width * height * 32 + 2] = baseX + cellWidth * ONE_PART;
    mappedVertexes[width * height * 32 + 3] = baseY + cellHeight * BIGGEST_PART;
    mappedVertexes[width * height * 32 + 4] = baseX + cellWidth * BIGGEST_PART;
    mappedVertexes[width * height * 32 + 5] = baseY + cellHeight * ONE_PART;
    mappedVertexes[width * height * 32 + 6] = baseX + cellWidth * ONE_PART;
    mappedVertexes[width * height * 32 + 7] = baseY + cellHeight * ONE_PART;
    glFlushMappedNamedBufferRange(vbo, vboLength - 4 * 2 * sizeof(float), 4 * 2 * sizeof(float));
    basic_Shader->bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, width * height * 4 * 4, 4);
}
