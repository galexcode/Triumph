//
//  TriumphGame.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "TriumphGame.h"

#include "Console.h"
#include "BattleScene.h"
#include "WorldScene.h"

TriumphGame::TriumphGame()
{
    Console::getInstance()->message(CONSOLE_MSG_SYS, "Starting Triumph");

	m_state = world;
    m_scene = new BattleScene();
    //m_scene = new WorldScene();
    
    m_engine = GameEngine::getInstance();
}

int TriumphGame::init()
{   
	// init scene
	m_scene->init();

	// init textures and meshes
	m_scene->load();

    return INIT_SUCCESS;
}

void TriumphGame::drawUI(float dTime) {

}

void TriumphGame::draw(float dTime) {
	m_scene->draw(dTime);
}

void TriumphGame::update(float dTime) {
	m_scene->update(dTime);
}

void TriumphGame::clean() {
	m_scene->clean();
}

