//
//  GameInput.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GameInput.h"
#include "TriumphGame.h"
#include <GL/glfw.h>

void GLFWCALL mouse_position_callback(int x, int y)
{
    TriumphGame::getInstance()->mouseMove(x, y);
}

void GLFWCALL window_size_callback(int width, int height)
{
    TriumphGame::getInstance()->windowResize(width, height);
}

void GLFWCALL keyboard_callback(int key, int state)
{
    TriumphGame::getInstance()->keyEvent(key, state);
}