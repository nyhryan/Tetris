#include "Game.h"

int main(int argc, char** argv)
{
    Game game;
    bool gameInit = game.Init();
    if (gameInit)
        game.RunLoop();
    game.Shutdown();

    return 0;
}