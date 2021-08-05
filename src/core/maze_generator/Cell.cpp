#include "Cell.h"

Cell::Cell() : borders(NORTH_SIDE | WEST_SIDE | SOUTH_SIDE | EAST_SIDE) {}

bool Cell::hasBorder(Side side) {
    return borders & side;
}

bool Cell::isBorderless() {
    return !(borders & (NORTH_SIDE | WEST_SIDE | SOUTH_SIDE | EAST_SIDE));
}

bool Cell::isBorderfull() {
    return borders & NORTH_SIDE && borders & WEST_SIDE && borders & SOUTH_SIDE && borders & EAST_SIDE;
}

void Cell::setBorder(Side side) {
    borders |= side;
}

void Cell::removeBorder(Side side) {
    borders = borders & ~side;
}
