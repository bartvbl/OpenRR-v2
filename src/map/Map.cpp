#include "Map.h"

openrr::MapTile* openrr::Map::at(unsigned int x, unsigned int y) {
    return &contents.at(x * height + y);
}
