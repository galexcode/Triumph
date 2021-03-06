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
#include "Gfx.h"

#include "GameInput.h"
#include "Texture.h"
#include "Game.h"
#include "Scene.h"
#include "Font.h"

#include <vector>

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

    bool m_fGLSupportedMultisample;
    bool m_fGLSupportedVBO;
    
    enum Mode {
		release,
		debug
	};

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
    
	void registerScene(Scene *scene);

    void mouseMove(int x, int y);
    void windowResize(int width, int height);
    void keyEvent(int key, int state);
	void mouseButtonEvent(int button, int state);
	void mouseWheelEvent(int dir);
    
    void set2D();
    void set3D();

	void setDebug(Mode mode);
	Mode getDebug();
    
protected:
    GameEngine();
    
private:
    static GameEngine *m_pInstance;
    
	enum Mode m_debugMode;

	Font *m_debugFont;

    void update(float dTime);
    void draw(float dTime);
    
	std::vector<Scene*> m_registered_scenes;
};

#endif /* defined(__Triumph__GameEngine__) */
