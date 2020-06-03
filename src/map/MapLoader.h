#pragma once

#include <ore/utilities/filesystem.h>
#include <json.hpp>
#include "Map.h"

namespace openrr {
    openrr::Map* loadMap(const ore::filesystem::path &location);
}