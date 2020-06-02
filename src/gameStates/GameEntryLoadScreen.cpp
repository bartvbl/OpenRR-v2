#include <ore/gl/vao/GeometryBufferGenerator.h>
#include "GameEntryLoadScreen.h"

void GameEntryLoadScreen::init(ore::resources::ResourceCache *cache) {
    //ore::resources::MeshResource* chrystal = cache->meshes.getResource_Blocking("chrystal_loadScreen");
    shader = cache->shaders.getResource_Blocking("loadscreenShader")->getInstance();

    ore::resources::Mesh mesh;
    ore::resources::MeshGeometry geometry;
    geometry.vertices = {
            ore::geom::vec3(0, 0, 0),
            ore::geom::vec3(1, 0, 0),
            ore::geom::vec3(1, 1, 0),
            ore::geom::vec3(0, 1, 0)};
    geometry.indices = {0, 1, 2, 0, 2, 3};
    planeBuffer = ore::gl::generateGeometryBuffer(geometry);
    std::cout << "Loading the loading screen!" << std::endl;
}

void GameEntryLoadScreen::draw(float progress) {
    shader.use();
    float mvpMatrix[16] {
            1.2f * progress, 0, 0, 0,
            0, 0.07, 0, 0,
            0, 0, 1, 0,
            -0.9, -0.9, 0, 1};
    shader.setUniform(6, mvpMatrix);
}

