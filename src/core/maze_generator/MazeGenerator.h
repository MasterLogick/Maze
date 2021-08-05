#ifndef MAZE_MAZEGENERATOR_H
#define MAZE_MAZEGENERATOR_H

#include <random>
#include <stack>
#include "../Point.h"
#include "Cell.h"

class MazeGenerator {
private:
    Cell **maze;
    int width;
    int height;
    std::mt19937 mtRandom;
    std::stack<Point> cells;
public:
    MazeGenerator(int width, int height);

    void nextStep();

    bool isComplete();

    Cell **getMaze();

    int getWidth();

    int getHeight();
};


#endif //MAZE_MAZEGENERATOR_H
