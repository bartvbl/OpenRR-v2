#pragma once

#include <vector>
#include "MapTile.h"

namespace openrr {
    class Map {
    private:
        std::vector<MapTile> contents;
    public:
        unsigned int width;
        unsigned int height;

        Map(unsigned int mapWidth, unsigned int mapHeight) : width(mapWidth), height(mapHeight) {
            contents.resize(width * height);
        }

        openrr::MapTile* at(unsigned int x, unsigned int y);
    };
}