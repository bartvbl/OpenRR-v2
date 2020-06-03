#include <iostream>
#include <map/MapResource.h>
#include <ore/sceneGraph/nodes/PerspectiveCamera.h>
#include "OpenRRGame.h"
#include "GameEntryLoadScreen.h"

void openrr::OpenRRGame::update() {

}

void openrr::OpenRRGame::set(ore::GameWorld* world) {
    std::cout << "Initialising game.." << std::endl;
    world->resourceCache.enqueueResourceFile("../res/resources.json");
    MapResource* mapResource = new openrr::MapResource();
    world->resourceCache.customResources.registerResource("GAME_MAP", ore::resources::ResourceLoadPriority::REQUIRED, "../res/maps/lrr/Level06/level06.json", mapResource);

    GameEntryLoadScreen loadingScreen;
    world->resourceCache.runLoadScreenSequence(&loadingScreen, ore::resources::ResourceLoadPriority::REQUIRED);


    ore::scene::PerspectiveCamera* camera = new ore::scene::PerspectiveCamera();
    world->scene.rootNode.getChildren()->push_back(camera);

}

void openrr::OpenRRGame::unset() {

}
