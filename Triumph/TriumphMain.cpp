#include "GameEngine.h"
#include "TriumphGame.h"

int main(void)
{
    
    GameEngine *engine = GameEngine::getInstance();
	engine->setDebug(GameEngine::Mode::debug);
    Game *game = new TriumphGame();
    
    if (engine->init(game) == INIT_SUCCESS)
    {
        engine->run();
    }
    engine->clean();

	delete game;
    
	exit(EXIT_SUCCESS);
}