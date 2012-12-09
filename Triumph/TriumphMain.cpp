#include "GameEngine.h"
#include "TriumphGame.h"

int main(void)
{
    
    GameEngine *engine = GameEngine::getInstance();
	engine->setDebug(GameEngine::debug);
    Game *game = new TriumphGame();
    
    if (engine->init(game) == INIT_SUCCESS)
    {
        engine->run();
    }
    engine->clean();
	game->clean();

	delete game;
    
	exit(EXIT_SUCCESS);
}
