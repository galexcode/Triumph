//
//  TriumphGame.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "TriumphGame.h"

#include "Console.h"
#include "WorldScene.h"


TriumphGame::TriumphGame()
{
    Console::getInstance()->message(CONSOLE_MSG_SYS, "Starting Triumph");

	m_state = world;
	m_scene = new WorldScene();

    m_engine = GameEngine::getInstance();
}

int TriumphGame::init()
{
    // init the debug font
	m_font = new Font("courier.bmp");
	m_font->init();
    
	// init scene
	m_scene->init();

	// init textures and meshes
	m_scene->load();

    return INIT_SUCCESS;
}

void TriumphGame::mouseButtonEvent(int button, int state) {

}
void TriumphGame::mouseWheelEvent(int dir) {

}

void TriumphGame::drawUI(float dTime) {
	char buf[256];
	sprintf(buf, "%d", (int)m_engine->m_fps);
	m_font->print(buf, m_engine->m_windowWidth - 30, m_engine->m_windowHeight - 20);
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

