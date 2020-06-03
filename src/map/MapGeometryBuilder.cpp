#include <stdexcept>
#include <array>
#include "MapGeometryBuilder.h"

enum class Orientation {
    NORTH, WEST, SOUTH, EAST
};

enum class WallType {
    CORNER,
    STRAIGHT,
    INVERTED_CORNER,
    CREVICE,
    FLAT,
    FLOOR
};

void wallNeighbourHoodToTileType(unsigned char neighbouringWalls, Orientation &orientation, WallType &wallType) {
    switch(neighbouringWalls) {
        case 1: orientation = Orientation::NORTH; wallType = WallType::CORNER; break;
        case 2: orientation = Orientation::WEST; wallType = WallType::CORNER; break;
        case 3: orientation = Orientation::NORTH; wallType = WallType::STRAIGHT; break;
        case 4: orientation = Orientation::SOUTH; wallType = WallType::CORNER; break;
        case 5: orientation = Orientation::WEST; wallType = WallType::CREVICE; break;
        case 6: orientation = Orientation::WEST; wallType = WallType::STRAIGHT; break;
        case 7: orientation = Orientation::WEST; wallType = WallType::INVERTED_CORNER; break;
        case 8: orientation = Orientation::EAST; wallType = WallType::CORNER; break;
        case 9: orientation = Orientation::EAST; wallType = WallType::STRAIGHT; break;
        case 10: orientation = Orientation::NORTH; wallType = WallType::CREVICE; break;
        case 11: orientation = Orientation::NORTH; wallType = WallType::INVERTED_CORNER; break;
        case 12: orientation = Orientation::SOUTH; wallType = WallType::STRAIGHT; break;
        case 13: orientation = Orientation::EAST; wallType = WallType::INVERTED_CORNER; break;
        case 14: orientation = Orientation::SOUTH; wallType = WallType::INVERTED_CORNER; break;
        case 15: orientation = Orientation::NORTH; wallType = WallType::FLAT; break;
        default: throw std::runtime_error("Invalid wall arrangement! Value: " + std::to_string(neighbouringWalls));
    }
}

bool isVertexHigh(openrr::Map *map, unsigned int x, unsigned int y) {
    return (!map->at(x, y)->isExposed) || map->at(x, y)->isWall;
}

void openrr::updateGeometryOfSingleTile(openrr::Map *map, unsigned int x, unsigned int y) {
    unsigned int tileBaseIndex = 6 * (x * map->height + y);
    MapTile* tile = map->at(x, y);

    const float tileWidth = float(map->metadata.tileWidth);
    const float wallHeight = float(map->metadata.wallHeight);

    Orientation orientation;
    WallType wallType;

    bool right = isVertexHigh(map, x + 1, y);
    bool topRight = isVertexHigh(map, x + 1, y + 1);
    bool top = isVertexHigh(map, x, y + 1);
    bool topLeft = isVertexHigh(map, x - 1, y + 1);
    bool left = isVertexHigh(map, x - 1, y);
    bool bottomLeft = isVertexHigh(map, x - 1, y - 1);
    bool bottom = isVertexHigh(map, x, y - 1);
    bool bottomRight = isVertexHigh(map, x + 1, y - 1);

    bool topRightTileCornerIsHigh = right && topRight && top;
    bool topLeftTileCornerIsHigh = top && topLeft && left;
    bool bottomLeftTileCornerIsHigh = left && bottomLeft && bottom;
    bool bottomRightTileCornerIsHigh = bottom && bottomRight && right;

    if(tile->isWall) {
        // Recovery for faulty map setup where single width walls exist
        // Removes the faulty wall, and simulates its collapse
        // This is exactly as the original game would handle this
        if(((left || right) && !top && !bottom) ||
           (!left && !right && (top || bottom))) {
            tile->isWall = false;
            tile->rubbleLevel = 4;
            tile->rubbleOreContent = 4;
            // Retry the update. This time it should succeed.
            updateGeometryOfSingleTile(map, x, y);
            return;
        }

        unsigned char wallNeighbourhood =
        (topRightTileCornerIsHigh ? 8U : 0U) |
        (topLeftTileCornerIsHigh ? 4U : 0U) |
        (bottomLeftTileCornerIsHigh ? 2U : 0U) |
        (bottomRightTileCornerIsHigh ? 1U : 0U);

        wallNeighbourHoodToTileType(wallNeighbourhood, orientation, wallType);
    } else {
        orientation = Orientation::NORTH;
        wallType = WallType::FLOOR;
    }

    std::array<unsigned int, 6> indexMapping = {0, 0, 0, 0, 0, 0};

    switch(orientation) {
        case Orientation::NORTH: indexMapping = {2, 3, 0, 2, 0, 1}; break;
        case Orientation::WEST:  indexMapping = {3, 0, 1, 3, 1, 2}; break;
        case Orientation::SOUTH: indexMapping = {0, 1, 2, 0, 2, 3}; break;
        case Orientation::EAST:  indexMapping = {1, 2, 3, 1, 3, 0}; break;
    }

    std::array<glm::vec3, 4> vertices = {
            glm::vec3(float(x) * tileWidth, float(y) * tileWidth, bottomLeftTileCornerIsHigh ? wallHeight : 0),
            glm::vec3(float(x + 1) * tileWidth, float(y) * tileWidth, bottomRightTileCornerIsHigh ? wallHeight : 0),
            glm::vec3(float(x + 1) * tileWidth, float(y + 1) * tileWidth, topRightTileCornerIsHigh ? wallHeight : 0),
            glm::vec3(float(x) * tileWidth, float(y + 1) * tileWidth, topLeftTileCornerIsHigh ? wallHeight : 0)
    };

    for(unsigned int i = 0; i < 6; i++) {
        map->geometry.vertices.at(tileBaseIndex + i) = vertices[indexMapping[i]];
        map->geometry.indices.at(tileBaseIndex + i) = tileBaseIndex + i;
    }
}

void openrr::buildMapGeometry(openrr::Map *map) {
    unsigned int mapVertexCount = 6 * map->width * map->height;

    map->geometry.hasNormals = true;
    map->geometry.hasTextures = true;

    map->geometry.vertices.resize(mapVertexCount);
    map->geometry.normals.resize(mapVertexCount);
    map->geometry.textureCoordinates.resize(mapVertexCount);
    map->geometry.indices.resize(mapVertexCount);

    for(unsigned int x = 0; x < map->width; x++) {
        for(unsigned int y = 0; y < map->height; y++) {
            updateGeometryOfSingleTile(map, x, y);
        }
    }
}
