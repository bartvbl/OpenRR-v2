#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <json.hpp>

void readMapComponentFile(const ore::filesystem::path &location, std::vector<unsigned short> &fileContents, unsigned int* mapWidth = nullptr, unsigned int* mapHeight = nullptr) {
    std::fstream inStream(location, std::ios::in | std::ios::binary);
    unsigned int header = 0;
    unsigned int byteCount = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    inStream.read((char*) &header, sizeof(unsigned int));
    inStream.read((char*) &byteCount, sizeof(unsigned int));
    inStream.read((char*) &width, sizeof(unsigned int));
    inStream.read((char*) &height, sizeof(unsigned int));
    if(mapWidth != nullptr) {
        *mapWidth = width - 1;
    }
    if(mapHeight != nullptr) {
        *mapHeight = height - 1;
    }

    unsigned int bufferSize = byteCount - 2 * sizeof(unsigned int);
    unsigned int bufferEntryCount = bufferSize / sizeof(unsigned short);

    fileContents.resize(bufferEntryCount);
    inStream.read((char*) fileContents.data(), bufferSize);
}


void applyHeightMap(std::vector<unsigned short> &heightMap, openrr::Map *map, unsigned int heightMultiplier) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            map->at(x, y)->tileHeight = heightMap.at(tileIndex) * heightMultiplier;
        }
    }
}

void applyPredugMap(std::vector<unsigned short> &predugMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned short dugValue = predugMap.at(tileIndex);
            switch(dugValue) {
                case 0:
                    map->at(x, y)->isWall = true;
                    map->at(x, y)->isExposed = false;
                    map->at(x, y)->containsSlimySlugHole = false;
                    break;
                case 1:
                    map->at(x, y)->isWall = false;
                    map->at(x, y)->isExposed = true;
                    map->at(x, y)->containsSlimySlugHole = false;
                    break;
                case 2:
                    map->at(x, y)->isWall = false;
                    map->at(x, y)->isExposed = false;
                    map->at(x, y)->containsSlimySlugHole = false;
                    break;
                case 3:
                    map->at(x, y)->isWall = false;
                    map->at(x, y)->isExposed = true;
                    map->at(x, y)->containsSlimySlugHole = true;
                    break;
                case 4:
                    map->at(x, y)->isWall = false;
                    map->at(x, y)->isExposed = false;
                    map->at(x, y)->containsSlimySlugHole = true;
                    break;
                default:
                    throw std::runtime_error("Predug map contains invalid value at tile " + std::to_string(x) + ", " + std::to_string(y));
            }
        }
    }
}

void applyCryoreMap(std::vector<unsigned short> &cryoreMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned short cryoreValue = cryoreMap.at(tileIndex);
            switch(cryoreValue) {
                case 0: break;
                case 1:
                case 3: map->at(x, y)->wallChrystalCount = 1; break;
                case 2:
                case 4: map->at(x, y)->wallOreCount = 1; break;
                case 5:
                case 7: map->at(x, y)->wallChrystalCount = 3; break;
                case 6:
                case 8: map->at(x, y)->wallOreCount = 3; break;
                case 9:
                case 11: map->at(x, y)->wallChrystalCount = 5; break;
                case 10:
                case 12: map->at(x, y)->wallOreCount = 5; break;
                case 13:
                case 15: map->at(x, y)->wallChrystalCount = 10; break;
                case 14:
                case 16: map->at(x, y)->wallOreCount = 10; break;
                case 17:
                case 19: map->at(x, y)->wallChrystalCount = 25; break;
                case 18:
                case 20: map->at(x, y)->wallOreCount = 25; break;
                default:
                    throw std::runtime_error("Cryore map contains invalid value at tile " + std::to_string(x) + ", " + std::to_string(y));
            }
        }
    }
}

void applyFallinMap(std::vector<unsigned short> &fallinMap, openrr::Map *map, unsigned int rateMultiplier) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned short fallinValue = fallinMap.at(tileIndex);
            map->at(x, y)->caveinRate = fallinValue * rateMultiplier;
        }
    }
}

void applyEmergeMap(std::vector<unsigned short> &emergeMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            // simplified version of the original, but this remake uses different monster spawn mechanics
            map->at(x, y)->isMonsterSpawnPoint = emergeMap.at(tileIndex) % 6 == 1;
        }
    }
}

void applyErosionMap(std::vector<unsigned short> &erodeMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned int erosionValue = erodeMap.at(tileIndex);
            if((erosionValue & 0x1U) == 0x0U) {
                map->at(x, y)->erosionRate = (erosionValue + 1) / 2;
            } else {
                map->at(x, y)->forcedErosionRate = erosionValue / 2;
            }
        }
    }
}

void applyPathMap(std::vector<unsigned short> &pathMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned int pathValue = pathMap.at(tileIndex);
            if(pathValue == 1) {
                map->at(x, y)->rubbleLevel = 4;
            }
            if(pathValue == 2) {
                map->at(x, y)->hasPowerPath = true;
            }
        }
    }
}

void applyTerrainMap(std::vector<unsigned short> &terrainMap, openrr::Map *map) {
    for(unsigned int y = 0; y < map->height; y++) {
        for(unsigned int x = 0; x < map->width; x++) {
            unsigned int tileIndex = x * map->height + y;
            unsigned int terrainValue = terrainMap.at(tileIndex);
            switch(terrainValue) {
                case 0: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                case 1: map->at(x, y)->soilType = openrr::SoilType::SOLID_ROCK; break;
                case 2: map->at(x, y)->soilType = openrr::SoilType::HARD_ROCK; break;
                case 3: map->at(x, y)->soilType = openrr::SoilType::LOOSE_ROCK; break;
                case 4: map->at(x, y)->soilType = openrr::SoilType::DIRT; break;
                case 5: map->at(x, y)->soilType = openrr::SoilType::SOIL; break;
                case 6: map->at(x, y)->soilType = openrr::SoilType::LAVA; break;
                case 7: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                case 8: map->at(x, y)->soilType = openrr::SoilType::ORE_SEAM; break;
                case 9: map->at(x, y)->soilType = openrr::SoilType::WATER; break;
                case 10: map->at(x, y)->soilType = openrr::SoilType::CHRYSTAL_SEAM; break;
                case 11: map->at(x, y)->soilType = openrr::SoilType::RECHARGE_SEAM; break;
                case 12: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                case 13: map->at(x, y)->soilType = openrr::SoilType::REINFORCED_WALL; break;
                case 14: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                case 15: map->at(x, y)->soilType = openrr::SoilType::SLIMY_SLUG_HOLE; break;
                case 16: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                case 17: map->at(x, y)->soilType = openrr::SoilType::NONE; break;
                default:
                    throw std::runtime_error("Terrain map contains invalid value at tile " + std::to_string(x) + ", " + std::to_string(y));
            }
        }
    }
}

openrr::Map *openrr::loadMap(const ore::filesystem::path &location) {
    nlohmann::json mapMetadata;
    std::fstream fileStream(location, std::ios::in);
    fileStream >> mapMetadata;

    fileStream.close();
    ore::filesystem::path containingDirectory = location.parent_path();

    unsigned int mapWidth = 0;
    unsigned int mapHeight = 0;
    std::vector<unsigned short> heightMap;
    std::vector<unsigned short> predugMap;
    std::vector<unsigned short> terrainMap;
    std::vector<unsigned short> cryOreMap;
    std::vector<unsigned short> fallinMap;
    std::vector<unsigned short> emergeMap;
    std::vector<unsigned short> erodeMap;
    std::vector<unsigned short> pathMap;
    std::vector<unsigned short> blockPointersMap;

    // Terrain map is necessary, so we use this one to determine the map width and height
    readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["terrainMap"],
            terrainMap, &mapWidth, &mapHeight);

    openrr::Map* map = new openrr::Map(mapWidth, mapHeight);

    applyTerrainMap(terrainMap, map);

    if(mapMetadata["mapFiles"]["heightMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["heightMap"],heightMap);
        applyHeightMap(heightMap, map, mapMetadata["dimensions"]["heightMapScaleFactor"]);
    }
    if(mapMetadata["mapFiles"]["predugMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["predugMap"],predugMap);
        applyPredugMap(predugMap, map);
    }
    if(mapMetadata["mapFiles"]["cryoreMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["cryoreMap"],cryOreMap);
        applyCryoreMap(cryOreMap, map);
    }
    if(mapMetadata["mapFiles"]["fallinMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["fallinMap"],fallinMap);
        applyFallinMap(fallinMap, map, mapMetadata["caveins"]["timeoutMultiplier"]);
    }
    if(mapMetadata["mapFiles"]["emergeMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["emergeMap"],emergeMap);
        applyEmergeMap(emergeMap, map);
    }
    if(mapMetadata["mapFiles"]["erodeMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["erodeMap"],erodeMap);
        applyErosionMap(erodeMap, map);
    }
    if(mapMetadata["mapFiles"]["pathMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["pathMap"],pathMap);
        applyPathMap(pathMap, map);
    }
    if(mapMetadata["mapFiles"]["blockPointersMap"] != "NONE") {
        readMapComponentFile(containingDirectory / mapMetadata["mapFiles"]["blockPointersMap"],blockPointersMap);
    }

    map->metadata.name = mapMetadata["name"];
    map->metadata.objectiveText = mapMetadata["objectiveText"];
    map->metadata.enableSpiders = mapMetadata["enableSpiders"];
    map->metadata.introVideoPath = mapMetadata["introVideo"];

    map->metadata.monsterSpawnEnabled = mapMetadata["monsterSpawns"]["enabled"];
    map->metadata.monsterSpawnCreature = mapMetadata["monsterSpawns"]["creature"];
    map->metadata.monsterSpawnTimeout = mapMetadata["monsterSpawns"]["timeout"];

    map->metadata.caveinsEnabled = mapMetadata["caveins"]["enabled"];
    map->metadata.caveinAnimation = mapMetadata["caveins"]["animation"];
    map->metadata.caveinTimeoutMultiplier = mapMetadata["caveins"]["timeoutMultiplier"];

    map->metadata.thumbnailRegular = mapMetadata["levelThumbnail"]["regular"];
    map->metadata.thumbnailHover = mapMetadata["levelThumbnail"]["hover"];
    map->metadata.thumbnailDisabled = mapMetadata["levelThumbnail"]["disabled"];

    map->metadata.tileWidth = mapMetadata["dimensions"]["tileWidth"];
    map->metadata.wallHeight = mapMetadata["dimensions"]["wallHeight"];
    map->metadata.heightMapScaleFactor = mapMetadata["dimensions"]["heightMapScaleFactor"];
    map->metadata.roofHeight = mapMetadata["dimensions"]["roofHeight"];
    map->metadata.selectionBoxHeight = mapMetadata["dimensions"]["selectionBoxHeight"];

    map->metadata.textureSet = mapMetadata["textureSet"];
    map->metadata.rockFallStyle = mapMetadata["rockFallStyle"];

    map->metadata.oxygenRate = mapMetadata["oxygenRate"];

    map->metadata.fogColour = glm::vec3(mapMetadata["fogColour"][0], mapMetadata["fogColour"][1], mapMetadata["fogColour"][2]);
    map->metadata.highFogColour = glm::vec3(mapMetadata["highFogColour"][0], mapMetadata["highFogColour"][1], mapMetadata["highFogColour"][2]);

    return map;
}
