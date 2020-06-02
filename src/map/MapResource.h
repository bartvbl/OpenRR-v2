#pragma once

#include <ore/resources/resourceTypes/ResourceType.h>

class MapResource : public ore::resources::ResourceType {
public:
    void load(const ore::filesystem::path &fileLocation) override;

    bool requiresMainThread() override;

    void completeLoadOnMainThread() override;

    void destroy() override;
};