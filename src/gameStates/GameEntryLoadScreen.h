#pragma once


#include <ore/resources/loadScreen/LoadScreenRenderer.h>

class GameEntryLoadScreen : public ore::resources::LoadScreenRenderer {
public:
    void init(ore::resources::ResourceCache *cache) override;
    void draw(float progress) override;
};

