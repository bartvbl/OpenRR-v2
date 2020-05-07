#include <ore/Ore.h>
#include <gameStates/OpenRRGame.h>

int main(int argc, char* argb[])
{
    ore::Engine engine;

    openrr::OpenRRGame mainGameState;

    engine.run("../res/config.json", &mainGameState);

    return 0;
}
