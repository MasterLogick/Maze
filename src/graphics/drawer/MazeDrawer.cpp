#include "MazeDrawer.h"
#include "../shader/Shaders.h"
#include "../shader/ShaderProgram.h"
#include <glad/gl.h>

#define CELL_SIDE 100.0f
#define PARTS 8.0f
#define ONE_PART CELL_SIDE * 1.0f / PARTS
#define BIGGEST_PART CELL_SIDE * (PARTS - 1.0f) / PARTS
#define CEIL_Y CELL_SIDE / 2.0f
#define FLOOR_Y -CELL_SIDE / 2.0f
#define WALL_VERTEXES_COUNT 6
#define VERTEX_SIZE 3
#define VERTEX_RAW_SIZE VERTEX_SIZE * sizeof(float)

MazeDrawer::MazeDrawer(MazeGenerator &mazeGenerator, int screenWidth, int screenHeight) : mazeGenerator(mazeGenerator),
                                                                                          width(mazeGenerator.getWidth()),
                                                                                          height(mazeGenerator.getHeight()),
                                                                                          screenWidth(screenWidth),
                                                                                          screenHeight(screenHeight),
                                                                                          vboLength(
                                                                                                  width * height * 4 *
                                                                                                  WALL_VERTEXES_COUNT *
                                                                                                  VERTEX_RAW_SIZE) {
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vboLength, nullptr,
                         GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_CLIENT_STORAGE_BIT);
    GLuint attribLoc = basic_Shader->getAttribLocation("localPos");
    glEnableVertexArrayAttrib(vao, attribLoc);
    glVertexArrayAttribFormat(vao, attribLoc, VERTEX_SIZE, GL_FLOAT, false, 0);
    glVertexArrayVertexBuffer(vao, attribLoc, vbo, 0, VERTEX_RAW_SIZE);
    mappedVertexes = static_cast<float *>(glMapNamedBufferRange(vbo, 0, vboLength,
                                                                GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT |
                                                                GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));
}

MazeDrawer::~MazeDrawer() {
    glDeleteVertexArrays(1, &vao);
    glUnmapNamedBuffer(vbo);
    glDeleteBuffers(1, &vbo);
}

// +-------------------->+X
// | +-+----------+-+
// | | |    S       |
// | + +----------+-+
// | | |          | |
// | | |          | |
// | | |          | |
// | | |          | |
// | |E|          |W|
// | | |          | |
// | | |          | |
// | | |          | |
// | | |          | |
// | | |       2,3| |
// | +-+----------+ +
// | |4,5   N     | |
// | +-+----------+-+
//\./            0,1
// +Z

// +-------------------->+X
// | +-+----------+-+
// | |E|          |S|
// | +-+          +-+
// | |              |
// | |              |
// | |              |
// | |              |
// | |              |
// | |              |
// | |              |
// | |              |
// | |              |
// | |4,5           |
// | +-+2,3       +-+
// | |N|          |W|
// | +-+----------+-+
//\./ 0,1
// +Z

void MazeDrawer::drawMaze() {
    Cell **maze = mazeGenerator.getMaze();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Cell &cell = maze[i][j];
            float baseX = i * CELL_SIDE;
            float baseZ = j * CELL_SIDE;
            if (cell.hasBorder(NORTH_SIDE)) {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 0] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 1] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 2] = baseZ + CELL_SIDE;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 3] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 4] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 5] = baseZ + CELL_SIDE;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 6] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 7] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 8] = baseZ + BIGGEST_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 9] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 10] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 11] = baseZ + BIGGEST_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 12] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 13] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 14] = baseZ + BIGGEST_PART;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 15] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 16] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 17] = baseZ + BIGGEST_PART;
            } else {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 0] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 1] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 2] = baseZ + CELL_SIDE;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 3] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 4] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 5] = baseZ + CELL_SIDE;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 6] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 7] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 8] = baseZ + BIGGEST_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 9] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 10] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 11] = baseZ + BIGGEST_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 12] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 13] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 14] = baseZ + BIGGEST_PART;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 15] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 16] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 17] = baseZ + BIGGEST_PART;
            }
            if (cell.hasBorder(EAST_SIDE)) {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 18] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 19] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 20] = baseZ + ONE_PART;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 21] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 22] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 23] = baseZ + ONE_PART;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 24] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 25] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 26] = baseZ + ONE_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 27] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 28] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 29] = baseZ + ONE_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 30] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 31] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 32] = baseZ + CELL_SIDE;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 33] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 34] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 35] = baseZ + CELL_SIDE;
            } else {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 18] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 19] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 20] = baseZ + BIGGEST_PART;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 21] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 22] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 23] = baseZ + BIGGEST_PART;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 24] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 25] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 26] = baseZ + BIGGEST_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 27] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 28] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 29] = baseZ + BIGGEST_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 30] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 31] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 32] = baseZ + CELL_SIDE;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 33] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 34] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 35] = baseZ + CELL_SIDE;
            }
            if (cell.hasBorder(SOUTH_SIDE)) {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 36] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 37] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 38] = baseZ;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 39] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 40] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 41] = baseZ;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 42] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 43] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 44] = baseZ + ONE_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 45] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 46] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 47] = baseZ + ONE_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 48] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 49] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 50] = baseZ + ONE_PART;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 51] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 52] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 53] = baseZ + ONE_PART;
            } else {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 36] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 37] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 38] = baseZ;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 39] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 40] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 41] = baseZ;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 42] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 43] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 44] = baseZ + ONE_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 45] = baseX + BIGGEST_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 46] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 47] = baseZ + ONE_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 48] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 49] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 50] = baseZ + ONE_PART;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 51] = baseX + CELL_SIDE;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 52] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 53] = baseZ + ONE_PART;
            }
            if (cell.hasBorder(WEST_SIDE)) {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 54] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 55] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 56] = baseZ + BIGGEST_PART;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 57] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 58] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 59] = baseZ + BIGGEST_PART;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 60] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 61] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 62] = baseZ + BIGGEST_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 63] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 64] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 65] = baseZ + BIGGEST_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 66] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 67] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 68] = baseZ;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 69] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 70] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 71] = baseZ;
            } else {
                //0
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 54] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 55] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 56] = baseZ + ONE_PART;
                //1
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 57] = baseX;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 58] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 59] = baseZ + ONE_PART;
                //2
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 60] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 61] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 62] = baseZ + ONE_PART;
                //3
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 63] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 64] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 65] = baseZ + ONE_PART;
                //4
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 66] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 67] = CEIL_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 68] = baseZ;
                //5
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 69] = baseX + ONE_PART;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 70] = FLOOR_Y;
                mappedVertexes[(i * height + j) * 4 * WALL_VERTEXES_COUNT * VERTEX_SIZE + 71] = baseZ;
            }
        }
    }
    glFlushMappedNamedBufferRange(vbo, 0, vboLength);
    border_Shader->bind();
    glBindVertexArray(vao);
    for (int i = 0; i < width * height * 4; ++i) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * WALL_VERTEXES_COUNT, WALL_VERTEXES_COUNT);
    }
}
