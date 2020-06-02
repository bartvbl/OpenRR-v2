#include <iostream>
#include "MapResource.h"
#include "MapLoader.h"

void openrr::MapResource::load(const ore::filesystem::path &fileLocation) {
    map = openrr::loadMap(fileLocation);
}

bool openrr::MapResource::requiresMainThread() {
    return true;
}

void openrr::MapResource::completeLoadOnMainThread() {

}

void openrr::MapResource::destroy() {

}
