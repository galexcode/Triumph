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

#define INPUTSTATE_PRESS   GLFW_PRESS
#define INPUTSTATE_RELEASE GLFW_RELEASE

#define INPUTBTN_MOUSELEFT   GLFW_MOUSE_BUTTON_LEFT
#define INPUTBTN_MOUSERIGHT  GLFW_MOUSE_BUTTON_RIGHT
#define INPUTBTN_MOUSEMIDDLE GLFW_MOUSE_BUTTON_MIDDLE

void GLFWCALL mouse_position_callback(int x, int y);
void GLFWCALL window_size_callback(int width, int height);
void GLFWCALL keyboard_callback(int key, int state);
void GLFWCALL mouse_button_callback(int button, int state);
void GLFWCALL mouse_wheel_callback(int pos);

class InputManager
{
public:
	bool m_fCmdDown;
	bool m_fShiftDown;
	bool m_fAltDown;
};

#endif /* defined(__Triumph__Input__) */
