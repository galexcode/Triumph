#include "GameEngine.h"
#include "TriumphGame.h"

int main(void)
{
    
    GameEngine *engine = GameEngine::getInstance();
    Game *game = new TriumphGame();
    
    if (engine->init(game) == INIT_SUCCESS)
    {
        engine->run();
    }
    engine->clean();
    
	exit(EXIT_SUCCESS);
}