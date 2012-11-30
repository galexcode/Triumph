//
//  GameEngine.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GameEngine.h"

#include "GLUtil.h"
#include "Console.h"

GameEngine* GameEngine::m_pInstance = NULL;

GameEngine* GameEngine::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new GameEngine();
    }
    return m_pInstance;
}

GameEngine::GameEngine()
{
	m_input = new InputManager();
	m_debugMode = release;
}

void GameEngine::setDebug(Mode mode)
{
	m_debugMode = mode;
}

GameEngine::Mode GameEngine::getDebug() {
	return m_debugMode;
}

int GameEngine::init(Game *game)
{
    if (!glfwInit()) {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to Initialize GLFW");
        return INIT_FAIL;
    }
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, FSAA_SAMPLES);
    
    if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 8, 8, GLFW_WINDOW)) {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to open GLFW window");
        return INIT_FAIL;
    }
    
    // get support levels
    m_fGLSupportedMultisample = glfwExtensionSupported("GL_ARB_multisample");
    m_fGLSupportedVBO = glfwExtensionSupported("GL_ARB_vertex_buffer_object");
    
    glfwSetWindowTitle(WINDOW_TITLE);
    glfwSwapInterval(1);
    
    // init OPENGL settings
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearDepth(1.0f);
	glEnable(GL_BLEND);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable (GL_CULL_FACE);
        
	// init the debug font
	m_debugFont = new Font("courier.bmp");
	m_debugFont->init();

    if (game == NULL || game->init() == INIT_FAIL)
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Game not initialized");
        return INIT_FAIL;
    }
    m_game = game;
    
    // glfw callbacks
    glfwSetWindowSizeCallback(window_size_callback);
    glfwSetMousePosCallback(mouse_position_callback);
    glfwSetKeyCallback(keyboard_callback);
	glfwSetMouseButtonCallback(mouse_button_callback);
	glfwSetMouseWheelCallback(mouse_wheel_callback);
    
    // init the visual console
    Console::getInstance()->initScreenDisplay();
    
    return INIT_SUCCESS;
}

void GameEngine::windowResize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    
    m_game->windowResize(width, height);
}

void GameEngine::registerScene(Scene *s) {
	m_registered_scenes.push_back(s);
}

void GameEngine::mouseMove(int x, int y)
{
    m_game->mouseMove(x, y);
	for (auto s : m_registered_scenes)
		s->mouseMove(x, y);
}

void GameEngine::keyEvent(int key, int state)
{
    m_game->keyEvent(key, state);
	for (auto s : m_registered_scenes)
		s->keyEvent(key, state);
}

void GameEngine::mouseButtonEvent(int button, int state)
{
    m_game->mouseButtonEvent(button, state);
	for (auto s : m_registered_scenes)
		s->mouseButtonEvent(button, state);
}

void GameEngine::mouseWheelEvent(int dir)
{
    m_game->mouseWheelEvent(dir);
	for (auto s : m_registered_scenes)
		s->mouseWheelEvent(dir);
}


void GameEngine::update(float dTime)
{
    m_input->update(dTime);
    m_game->update(dTime);
    Console::getInstance()->update(dTime);
    
    m_input->afterUpdate();
}

void GameEngine::draw(float dTime)
{
    m_game->draw(dTime);

    set2D();
	if (m_debugMode == debug) {
		char buf[256];
		sprintf(buf, "%d", (int)m_fps);
		m_debugFont->print(buf, m_windowWidth - 30, m_windowHeight - 20);
		Console::getInstance()->draw(dTime);
	}
	m_game->drawUI(dTime);
}

void GameEngine::set3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45, (double)m_windowWidth / m_windowHeight, 0.1, 10000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GameEngine::set2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_windowWidth, 0, m_windowHeight, -1.0, 1.0);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GameEngine::run()
{
	m_elapsedTime = (float) glfwGetTime();
    
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        float dTime = (float) glfwGetTime() - m_elapsedTime;
		m_fps = 1 / dTime;
        m_elapsedTime += dTime;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        update(dTime);
        
        draw(dTime);
        
        glfwSwapBuffers();
    }
}

void GameEngine::getWindowSize(int *w, int *h)
{
    *w = m_windowWidth;
    *h = m_windowHeight;
}

float GameEngine::getElapsedTime()
{
    return m_elapsedTime;
}

void GameEngine::clean()
{
    m_game->clean();
    delete m_game;
    
    glfwTerminate();
    
    // kill singleton
    delete m_pInstance;
}

