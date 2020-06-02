#pragma once


#include <ore/resources/loadScreen/LoadScreenRenderer.h>

class GameEntryLoadScreen : public ore::resources::LoadScreenRenderer {
private:
    ore::resources::Shader shader;
    ore::gl::GeometryBuffer planeBuffer;
public:
    void init(ore::resources::ResourceCache *cache) override;
    void draw(float progress) override;
};

