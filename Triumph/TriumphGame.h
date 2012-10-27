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

#define SOL_PLATFORM_WIN32 0
#define SOL_PLATFORM_MAC32 1

#if defined(__WIN32__)
#define SOL_PLATFORM SOL_PLATFORM_WIN32
#elif defined(__APPLE__)
#define SOL_PLATFORM SOL_PLATFORM_MAC32
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Triumph"

#define FSAA_SAMPLES 4
#ifndef GL_ARB_multisample
#define GL_MULTISAMPLE_ARB 0x809D
#endif

#define INIT_SUCCESS 0
#define INIT_FAIL 1

#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

#include "GameInput.h"
#include "Texture.h"

class TriumphGame
{
public:
	InputManager *m_pInputManager;

    static TriumphGame* getInstance();
    int init();
    void run();
    void clean();
    
    void getWindowSize(int *w, int *h);
    float getElapsedTime();
    
    void mouseMove(int x, int y);
    void windowResize(int width, int height);
    void keyEvent(int key, int state);
    
protected:
    TriumphGame();
    
private:
    static TriumphGame *m_pInstance;
    
    int m_meshGlobe;
    Texture *m_texGlobe;
    
    int m_cursorX, m_cursorY;
    int m_windowWidth, m_windowHeight;
    float m_elapsedTime;
    
    void update(float dTime);
    void draw(float dTime);
    
};


#endif /* defined(__Triumph__TriumphGame__) */