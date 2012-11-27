//
//  TriumphGame.h
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//
//  Singleton game class
//  contains all engine components for access anywhere in the game subsystems

#ifndef __Triumph__TriumphGame__
#define __Triumph__TriumphGame__

#include "Game.h"
#include "GameEngine.h"
#include "Scene.h"

class TriumphGame : public Game
{
public:

    TriumphGame();
    
    int init();
    void clean();
    
    void update(float dTime);
    void drawUI(float dTime);
    void draw(float dTime);
    
private:
    
    GameEngine *m_engine;
    
	enum ENState {
		world,
		battle
	};
	enum ENState m_state;

	Scene *m_scene;
    
    
};


#endif /* defined(__Triumph__TriumphGame__) */