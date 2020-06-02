#include "MapLoader.h"
#include <fstream>
#include <iostream>

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
        *mapWidth = width;
    }
    if(mapHeight != nullptr) {
        *mapHeight = height;
    }

    unsigned int bufferSize = byteCount - 2 * sizeof(unsigned int);
    unsigned int bufferEntryCount = bufferSize / sizeof(unsigned short);

    fileContents.resize(bufferEntryCount);
    inStream.read((char*) fileContents.data(), bufferSize);
}

openrr::Map *openrr::loadMap(const ore::filesystem::path &location) {
    // Compute the base map name
    std::string fileName = location.filename();
    size_t dotIndex = fileName.find('.');
    fileName = fileName.substr(0, dotIndex);
    ore::filesystem::path containingDirectory = location.parent_path();

    unsigned int mapWidth = 0;
    unsigned int mapHeight = 0;
    std::vector<unsigned short> cryOre;
    readMapComponentFile(containingDirectory / ("Cror_" + fileName + ".map"), cryOre, &mapWidth, &mapHeight);
    std::cout << "Map " + (containingDirectory / ("Cror_" + fileName + ".map")).string() + " size is " + std::to_string(mapWidth) + "x" + std::to_string(mapHeight) + "\n" << std::flush;
    return nullptr;
}
