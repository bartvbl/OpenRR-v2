#include "Map.h"

openrr::MapTile* openrr::Map::at(int x, int y) {
    if(x < 0) return &outOfRangeTile;
    if(y < 0) return &outOfRangeTile;
    if(x >= (int) width) return &outOfRangeTile;
    if(y >= (int) height) return &outOfRangeTile;
    return &contents.at(x * height + y);
}

openrr::MapTile* openrr::Map::at(unsigned int x, unsigned int y) {
    return at(int(x), int(y));
}
