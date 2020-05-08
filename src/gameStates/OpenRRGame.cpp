#include <iostream>
#include "OpenRRGame.h"
#include "GameEntryLoadScreen.h"

void openrr::OpenRRGame::update() {

}

void openrr::OpenRRGame::set(ore::resources::ResourceCache *cache) {
    std::cout << "Initialising game.." << std::endl;
    cache->enqueueResourceFile("../res/resources.json");

    GameEntryLoadScreen loadingScreen;
    cache->runLoadScreenSequence(&loadingScreen, ore::resources::ResourceLoadPriority::REQUIRED);
}

void openrr::OpenRRGame::unset() {

}
