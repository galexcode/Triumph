//
//  GameInput.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GameInput.h"
#include "GameEngine.h"
#include <GL/glfw.h>

InputManager::InputManager() {
    for (int i = 0; i < NUMBEROFKEYS; ++i) {
        m_fKeys[i] = false;
    }
    m_lastWheel = 0;
    m_fMouseLeft = false;
    m_fMouseMiddle = false;
    m_fMouseRight = false;
    
	m_fLeft = false;
	m_fRight = false;
	m_fUp = false;
	m_fDown = false;

    m_fAlt = false;
    m_fCmd = false;
    m_fShift = false;
}

void InputManager::update(float dTime) {
    m_lastCursorX = m_cursorX;
    m_lastCursorY = m_cursorY;
    m_lastWheel = m_wheel;
}

void GLFWCALL mouse_position_callback(int x, int y) {
    GameEngine *engine = GameEngine::getInstance();
    engine->m_input->m_cursorX = x;
	engine->m_input->m_cursorY = y;
    
    engine->mouseMove(x, y);
}

void GLFWCALL window_size_callback(int width, int height)
{
    GameEngine::getInstance()->windowResize(width, height);
}

void GLFWCALL keyboard_callback(int key, int state)
{
    GameEngine *engine = GameEngine::getInstance();
    
    engine->m_input->m_fKeys[key] = state;
    
    if (key == (int)'A')
        engine->m_input->m_fLeft = state;
    else if (key == (int)'D')
        engine->m_input->m_fRight = state;
    else if (key == (int)'W')
        engine->m_input->m_fUp = state;
    else if (key == (int)'S')
        engine->m_input->m_fDown = state;
    
    engine->keyEvent(key, state);
}

void GLFWCALL mouse_button_callback(int button, int state)
{
    GameEngine *engine = GameEngine::getInstance();
	switch (button)
	{
		case INPUTBTN_MOUSELEFT:
		{
			engine->m_input->m_fMouseLeft = state == INPUTSTATE_PRESS;
		}
        case INPUTBTN_MOUSEMIDDLE:
        {
            engine->m_input->m_fMouseMiddle = state == INPUTBTN_MOUSEMIDDLE;
        }
        case INPUTBTN_MOUSERIGHT:
        {
            engine->m_input->m_fMouseRight = state == INPUTBTN_MOUSERIGHT;
        }
	}
    
	engine->mouseButtonEvent(button, state);
}
void GLFWCALL mouse_wheel_callback(int pos)
{
    GameEngine *engine = GameEngine::getInstance();
    int dir = pos - engine->m_input->m_lastWheel;
    
    engine->m_input->m_wheel = pos;
	engine->mouseWheelEvent(dir);
}
