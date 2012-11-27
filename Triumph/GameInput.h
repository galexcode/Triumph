//
//  GameInput.h
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__GameInput__
#define __Triumph__GameInput__

#include <GL/glfw.h>
#include "Game.h"

#define INPUTSTATE_PRESS   GLFW_PRESS
#define INPUTSTATE_RELEASE GLFW_RELEASE

#define INPUTBTN_MOUSELEFT   GLFW_MOUSE_BUTTON_LEFT
#define INPUTBTN_MOUSERIGHT  GLFW_MOUSE_BUTTON_RIGHT
#define INPUTBTN_MOUSEMIDDLE GLFW_MOUSE_BUTTON_MIDDLE

#define INPUTKEY_SPACE GLFW_KEY_SPACE

#define NUMBEROFKEYS 256

void GLFWCALL mouse_position_callback(int x, int y);
void GLFWCALL window_size_callback(int width, int height);
void GLFWCALL keyboard_callback(int key, int state);
void GLFWCALL mouse_button_callback(int button, int state);
void GLFWCALL mouse_wheel_callback(int pos);

class InputManager
{
public:
	bool m_fCmd;
	bool m_fShift;
	bool m_fAlt;
    
    bool m_fMouseLeft;
    bool m_fMouseMiddle;
    bool m_fMouseRight;
    
    bool m_fLeft;
    bool m_fRight;
    bool m_fUp;
    bool m_fDown;
    
    int m_lastWheel;
    int m_wheel;
    
    bool m_fKeys[256];
    
    int m_cursorX, m_cursorY;
	int m_lastCursorX, m_lastCursorY;
    
    InputManager();
    void update(float dTime);
};

#endif /* defined(__Triumph__Input__) */
