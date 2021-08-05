#ifndef MAZE_CELL_H
#define MAZE_CELL_H

#define NORTH_SIDE 0b1
#define WEST_SIDE 0b10
#define SOUTH_SIDE 0b100
#define EAST_SIDE 0b1000

typedef char8_t Side;

class Cell {
private:
    char8_t borders;
public:
    Cell();

    bool hasBorder(Side side);

    bool isBorderless();

    bool isBorderfull();

    void setBorder(Side side);

    void removeBorder(Side side);
};

#endif //MAZE_CELL_H
