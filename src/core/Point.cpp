#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

Point Point::move(Side side) {
    switch (side) {
        case NORTH_SIDE:
            return Point(x, y + 1);
        case WEST_SIDE:
            return Point(x - 1, y);
        case SOUTH_SIDE:
            return Point(x, y - 1);
        case EAST_SIDE:
            return Point(x + 1, y);
        default:
            return Point(-1, -1);
    }
}

Side Point::orientation(Point to) {
    if (y < to.y)return NORTH_SIDE;
    if (x > to.x)return WEST_SIDE;
    if (y > to.y)return SOUTH_SIDE;
    if (x < to.x)return EAST_SIDE;
    return 0;
}
