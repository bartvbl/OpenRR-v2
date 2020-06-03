#pragma once

#include <vector>
#include <ore/resources/resourceTypes/mesh/MeshGeometry.h>
#include <ore/gl/vao/GeometryBuffer.h>
#include "MapTile.h"
#include "MapMetadata.h"

namespace openrr {
    class Map {
    private:
        std::vector<MapTile> contents;
        openrr::MapTile outOfRangeTile;
    public:
        unsigned int width;
        unsigned int height;
        ore::resources::MeshGeometry geometry;
        ore::gl::GeometryBuffer geometryBuffer;
        openrr::MapMetadata metadata;

        Map(unsigned int mapWidth, unsigned int mapHeight) : width(mapWidth), height(mapHeight) {
            contents.resize(width * height);
            outOfRangeTile.isWall = true;
        }

        openrr::MapTile* at(int x, int y);
        openrr::MapTile* at(unsigned int x, unsigned int y);
    };
}