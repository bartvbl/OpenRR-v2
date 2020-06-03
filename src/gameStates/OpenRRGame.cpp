#include <iostream>
#include <map/MapResource.h>
#include "OpenRRGame.h"
#include "GameEntryLoadScreen.h"

void openrr::OpenRRGame::update() {

}

void openrr::OpenRRGame::set(ore::GameWorld* world) {
    std::cout << "Initialising game.." << std::endl;
    world->resourceCache.enqueueResourceFile("../res/resources.json");
    world->resourceCache.customResources.registerResource("GAME_MAP", ore::resources::ResourceLoadPriority::REQUIRED, "../res/maps/lrr/Level06/level06.json", new openrr::MapResource());

    GameEntryLoadScreen loadingScreen;
    world->resourceCache.runLoadScreenSequence(&loadingScreen, ore::resources::ResourceLoadPriority::REQUIRED);
}

void openrr::OpenRRGame::unset() {

}
