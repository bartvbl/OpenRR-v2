#include <iostream>
#include <ore/gl/vao/GeometryBufferGenerator.h>
#include "MapResource.h"
#include "MapLoader.h"
#include "MapGeometryBuilder.h"

void openrr::MapResource::load(const ore::filesystem::path &fileLocation) {
    map = openrr::loadMap(fileLocation);
    openrr::buildMapGeometry(map);
}

bool openrr::MapResource::requiresMainThread() {
    return true;
}

void openrr::MapResource::completeLoadOnMainThread() {
    map->geometryBuffer = ore::gl::generateGeometryBuffer(map->geometry);
}

void openrr::MapResource::destroy() {

}
