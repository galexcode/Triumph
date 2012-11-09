//
//  GameEngine.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__GameEngine__
#define __Triumph__GameEngine__

#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

#include "GameInput.h"
#include "Texture.h"
#include "Game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Triumph"

#define FSAA_SAMPLES 4
#ifndef GL_ARB_multisample
#define GL_MULTISAMPLE_ARB 0x809D
#endif

#define INIT_SUCCESS 0
#define INIT_FAIL 1

class GameEngine
{
public:
	InputManager *m_input;
    Game         *m_game;
	float m_fps;
    
    int m_windowWidth, m_windowHeight;
    float m_elapsedTime;
    
    static GameEngine* getInstance();
    int init(Game *game);
    void run();
    void clean();
    
    void getWindowSize(int *w, int *h);
    float getElapsedTime();
    
    void mouseMove(int x, int y);
    void windowResize(int width, int height);
    void keyEvent(int key, int state);
	void mouseButtonEvent(int button, int state);
	void mouseWheelEvent(int dir);
    
    void set2D();
    void set3D();
    
protected:
    GameEngine();
    
private:
    static GameEngine *m_pInstance;
    
    void update(float dTime);
    void draw(float dTime);
    
};

#endif /* defined(__Triumph__GameEngine__) */
