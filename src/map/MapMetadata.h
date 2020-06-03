#pragma once

#include <string>
#include <glm/glm.hpp>

namespace openrr {
    struct MapMetadata {
        std::string name;
        std::string objectiveText;
        bool enableSpiders = false;
        std::string introVideoPath;

        double monsterSpawnTimeout = 2500;
        std::string monsterSpawnCreature;
        bool monsterSpawnEnabled = false;

        std::string caveinAnimation;
        double caveinTimeoutMultiplier = 0;
        bool caveinsEnabled = false;

        std::string thumbnailRegular;
        std::string thumbnailHover;
        std::string thumbnailDisabled;

        double tileWidth = 40;
        double wallHeight = 40;
        double heightMapScaleFactor = 6;
        double roofHeight = 40;
        double selectionBoxHeight = 10;

        double oxygenRate = 0;

        double erodeTriggerTime = 0;
        double erosionTime = 0;
        double erosionLockTime = 0;

        std::string textureSet;
        std::string rockFallStyle;

        glm::vec3 fogColour = {255, 255, 255};
        glm::vec3 highFogColour = {255, 255, 255};
    };
}