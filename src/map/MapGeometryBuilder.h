#pragma once

#include <ore/resources/resourceTypes/mesh/MeshGeometry.h>
#include "Map.h"

namespace openrr {
    void updateGeometryOfSingleTile(openrr::Map* map, unsigned int x, unsigned int y);
    void buildMapGeometry(openrr::Map* map);
}