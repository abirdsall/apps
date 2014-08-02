#include "os.h"
#include "game.h"

int main(int argc, char *argv[])
{
    return os::Main( argc, argv,
                    gameInit,
                    gameTick,
                    gameTouch,
                    gameDraw,
                    gameKill,
                    os::WindowFormat() );
}
