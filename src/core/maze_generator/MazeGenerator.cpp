#include "MazeGenerator.h"
#include <chrono>

Side oppositeSide(Side side) {
    switch (side) {
        case NORTH_SIDE:
            return SOUTH_SIDE;
        case SOUTH_SIDE:
            return NORTH_SIDE;
        case EAST_SIDE:
            return WEST_SIDE;
        case WEST_SIDE:
            return EAST_SIDE;
        default:
            return 0;
    }
}


MazeGenerator::MazeGenerator(int width, int height) : width(width), height(height), mtRandom(), cells() {
    unsigned long time = std::chrono::system_clock::now().time_since_epoch().count();
    mtRandom.seed(time);
    maze = new Cell *[width];
    for (int i = 0; i < width; ++i) {
        maze[i] = new Cell[height];
        for (int j = 0; j < height; ++j) {
            maze[i][j] = Cell();
        }
    }
    cells.push(Point(0, 0));
}

void MazeGenerator::nextStep() {
    if (!cells.empty()) {
        Point current = cells.top();
        Point *nextVariants[4];
        int variants = 0;
        if (current.y != height - 1) {
            Point next = current.move(NORTH_SIDE);
            if (maze[next.x][next.y].isBorderfull()) {
                nextVariants[variants] = new Point(next);
                variants++;
            }
        }
        if (current.x != 0) {
            Point next = current.move(WEST_SIDE);
            if (maze[next.x][next.y].isBorderfull()) {
                nextVariants[variants] = new Point(next);
                variants++;
            }
        }
        if (current.y != 0) {
            Point next = current.move(SOUTH_SIDE);
            if (maze[next.x][next.y].isBorderfull()) {
                nextVariants[variants] = new Point(next);
                variants++;
            }
        }
        if (current.x != width - 1) {
            Point next = current.move(EAST_SIDE);
            if (maze[next.x][next.y].isBorderfull()) {
                nextVariants[variants] = new Point(next);
                variants++;
            }
        }
        if (variants != 0) {
            Point next = *nextVariants[mtRandom() % variants];
            Side side = current.orientation(next);
            maze[current.x][current.y].removeBorder(side);
            maze[next.x][next.y].removeBorder(oppositeSide(side));
            for (int i = 0; i < variants; ++i) {
                delete nextVariants[i];
            }
            cells.push(next);
        } else {
            cells.pop();
        }
    }
}

bool MazeGenerator::isComplete() {
    return cells.empty();
}

Cell **MazeGenerator::getMaze() {
    return maze;
}

int MazeGenerator::getWidth() {
    return width;
}

int MazeGenerator::getHeight() {
    return height;
}
