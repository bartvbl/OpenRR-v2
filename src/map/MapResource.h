#pragma once

#include <ore/resources/resourceTypes/ResourceType.h>
#include "Map.h"

namespace openrr {
    class MapResource : public ore::resources::ResourceType {
    public:
        Map* map;

        void load(const ore::filesystem::path &fileLocation) override;

        bool requiresMainThread() override;

        void completeLoadOnMainThread() override;

        void destroy() override;
    };
}
