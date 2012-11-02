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
#include "Font.h"
#include "GameObject.h"
#include "GameCamera.h"

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
	void mouseButtonEvent(int button, int state);
	void mouseWheelEvent(int pos);

	void set2D();
	void set3D();
    
protected:
    TriumphGame();
    
private:
    static TriumphGame *m_pInstance;
    
	float m_fps;

    int m_meshGlobe;
    int m_meshSkybox;
	float m_meshGlobeRot;
	float m_meshGlobeZoom;
    Texture *m_texGlobe;
    Texture *m_texSkybox;
    GameObject *m_globe;
    GameObject *m_skybox;
    Vector3 m_dragGlobeStart;
    
	GameCamera m_cam;

	Font *m_font;
    
    int m_cursorX, m_cursorY;
	int m_lastCursorX, m_lastCursorY;
	int m_lastWheel;
	bool m_fLeftMouseDown;
    bool m_fLeftDown;
    bool m_fRightDown;
    bool m_fUpDown;
    bool m_fDownDown;

    int m_windowWidth, m_windowHeight;
    float m_elapsedTime;
    
    void initGlobe();
    void initSkybox();
    
    void update(float dTime);
    void updateCamera();
    void draw(float dTime);
	void drawUI(float dTime);
    
    Ray3 getMouseRay();
    
};


#endif /* defined(__Triumph__TriumphGame__) */