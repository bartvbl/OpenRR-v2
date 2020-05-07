#include <ore/Ore.h>

int main(int argc, char* argb[])
{
    ore::Engine engine;

    ore::GameState gameState;

    engine.run("../res/config.json", &gameState);

    return 0;
}
