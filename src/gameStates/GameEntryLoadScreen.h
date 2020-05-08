#pragma once


#include <ore/resouces/loadScreen/LoadScreenRenderer.h>

class GameEntryLoadScreen : public ore::resources::LoadScreenRenderer {
    void init(ore::resources::ResourceCache *cache) override;
    void draw(float progress) override;
};

