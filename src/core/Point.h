#ifndef MAZE_POINT_H
#define MAZE_POINT_H

#include "maze_generator/Cell.h"

struct Point {
    int x, y;

    Point(int x, int y);

    Point move(Side side);

    Side orientation(Point to);
};


#endif //MAZE_POINT_H
