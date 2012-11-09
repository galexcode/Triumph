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

#define BACK_ID 0
#define FRONT_ID 1
#define RIGHT_ID 2
#define LEFT_ID 3
#define TOP_ID 4
#define BOTTOM_ID 5

#include <GL/glfw.h>

#include "Game.h"
#include "Texture.h"
#include "Font.h"
#include "GameObject.h"
#include "GameCamera.h"
#include "GameEngine.h"

class TriumphGame : public Game
{
public:

    TriumphGame();
    
    int init();
    void clean();
    
    void update(float dTime);
    void drawUI(float dTime);
    void draw(float dTime);
    
	void mouseButtonEvent(int button, int state);
	void mouseWheelEvent(int dir);
    
private:
    
    GameEngine *m_engine;

    int m_meshGlobe;
	float m_meshGlobeZoom;
    Texture *m_texGlobe;
    Texture *m_texSkybox[6];
    GameObject *m_globe;
    Vector3 m_dragGlobeStart;
    
	GameCamera m_cam;

	Font *m_font;
    
    void initGlobe();
    void initSkybox();
    
    void updateCamera();
    
    void drawSkybox(float dTime);
	
    
    Ray3 getMouseRay();
    
};


#endif /* defined(__Triumph__TriumphGame__) */