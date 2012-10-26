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

void GLFWCALL mouse_position_callback(int x, int y);
void GLFWCALL window_size_callback(int width, int height);
void GLFWCALL keyboard_callback(int key, int state);

class InputManager
{
public:
	bool m_fCmdDown;
	bool m_fShiftDown;
	bool m_fAltDown;
};

#endif /* defined(__Triumph__Input__) */
