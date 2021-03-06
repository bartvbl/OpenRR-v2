#pragma once

#include "SoilType.h"

namespace openrr {
    struct MapTile {
        unsigned int wallChrystalCount = 0;
        unsigned int wallOreCount = 0;
        unsigned int erosionRate = 0;
        unsigned int forcedErosionRate = 0;
        unsigned int caveinRate = 0;
        unsigned int rubbleLevel = 0;
        unsigned int rubbleOreContent = 0;
        unsigned int tileHeight = 0;
        bool isMonsterSpawnPoint = false;
        bool hasPowerPath = false;
        bool powerPathIsSquare = false;
        bool isExposed = false;
        bool isWall = false;
        bool containsSlimySlugHole = false;
        openrr::SoilType soilType = SoilType::NONE;
    };
}