#include "TriumphGame.h"

int main(void)
{
    
    TriumphGame *game = TriumphGame::getInstance();
    
    if (game->init() == INIT_SUCCESS)
    {
        game->run();
    }
    game->clean();
    
	exit(EXIT_SUCCESS);
}