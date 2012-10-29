//
//  TriumphGame.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "TriumphGame.h"
#include <GL/glfw.h>

#include "Console.h"

TriumphGame* TriumphGame::m_pInstance = NULL;

TriumphGame* TriumphGame::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new TriumphGame();
    }
    return m_pInstance;
}

TriumphGame::TriumphGame()
{
    Console::getInstance()->message(CONSOLE_MSG_SYS, "Starting Triumph");

	m_pInputManager = new InputManager();
	m_fLeftMouseDown = false;
	m_lastWheel = 0;
	m_meshGlobeZoom = 0;
}

void TriumphGame::windowResize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;

    glViewport(0, 0, m_windowWidth, m_windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, m_windowWidth, 0.f, m_windowHeight);
}

void TriumphGame::mouseMove(int x, int y)
{
    m_cursorX = x;
    m_cursorY = y;
}

void TriumphGame::keyEvent(int key, int state)
{
    Console::getInstance()->message(CONSOLE_MSG_SYS, "%d %d", key, state);
}

void TriumphGame::mouseButtonEvent(int button, int state)
{
	switch (button)
	{
		case INPUTBTN_MOUSELEFT:
		{
			m_fLeftMouseDown = state == INPUTSTATE_PRESS;
		}
	}
}

void TriumphGame::mouseWheelEvent(int pos)
{
	Console::getInstance()->message(CONSOLE_MSG_SYS, "%d", pos);
	m_meshGlobeZoom += pos - m_lastWheel;
	m_lastWheel = pos;
}

int TriumphGame::init()
{
    
    if (!glfwInit())
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to Initialize GLFW");
        return INIT_FAIL;
    }
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, FSAA_SAMPLES);
    
    if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to open GLFW window");
        return INIT_FAIL;
    }
    
    if (!glfwExtensionSupported("GL_ARB_multisample"))
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Context reports GL_ARB_multisample is not supported");
        return INIT_FAIL;
    }
    
    glfwSetWindowTitle(WINDOW_TITLE);
    glfwSetWindowSizeCallback(window_size_callback);
    glfwSetMousePosCallback(mouse_position_callback);
    glfwSetKeyCallback(keyboard_callback);
	glfwSetMouseButtonCallback(mouse_button_callback);
	glfwSetMouseWheelCallback(mouse_wheel_callback);
    glfwSwapInterval(1);
    
    int samples = glfwGetWindowParam(GLFW_FSAA_SAMPLES);
    if (samples)
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "%s %i", "Context reports FSAA sample size is", samples);
    }

    glClearColor(0, 0, 0, 0);
	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable ( GL_CULL_FACE );
    
    Console::getInstance()->initScreenDisplay();

	m_font = new Font("courier.bmp");
	m_font->init();
    
    m_texGlobe = Texture::CreateFromFile("earth_hires.bmp");
    
    // prep earth sphere display list
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
    m_meshGlobe = glGenLists(1);
    glNewList(m_meshGlobe, GL_COMPILE);
    gluSphere(sphere, 0.5, 200, 200);
    glEndList();
    gluDeleteQuadric(sphere);
	m_meshGlobeRot = 0;
    
    return INIT_SUCCESS;
}

void TriumphGame::drawUI(float dTime)
{
    glColor3f(1.f, 1.f, 1.f);
    
    glBegin(GL_LINES);
    glVertex2f(0.f, (GLfloat) m_windowHeight - m_cursorY);
    glVertex2f((GLfloat) m_windowWidth, (GLfloat) m_windowHeight - m_cursorY);
    glVertex2f((GLfloat) m_cursorX, 0.f);
    glVertex2f((GLfloat) m_cursorX, (GLfloat) m_windowHeight);
    glEnd();

	char buf[256];
	sprintf(buf, "%d", (int)m_fps);
	m_font->print(buf, m_windowWidth - 30, m_windowHeight - 20);
}

void TriumphGame::draw(float dTime)
{   
    glTranslatef(0, 0, -2 + m_meshGlobeZoom / 20);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glRotatef(m_meshGlobeRot, 0.0f, 0.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
    glCallList(m_meshGlobe);
    glDisable(GL_TEXTURE_2D);
}

void TriumphGame::update(float dTime)
{
    if (m_fLeftMouseDown)
	{
		m_meshGlobeRot += (float)(m_cursorX - m_lastCursorX);
	}

	m_lastCursorX = m_cursorX;
	m_lastCursorY = m_cursorY;
}

void TriumphGame::set3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TriumphGame::set2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_windowWidth, 0, m_windowHeight, -1.0, 1.0);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TriumphGame::run()
{	
	m_elapsedTime = (float) glfwGetTime();
    
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        float dTime = (float) glfwGetTime() - m_elapsedTime;
		m_fps = 1 / dTime;
        m_elapsedTime += dTime;
        Console *console = Console::getInstance();
        
        update(dTime);
        console->update(dTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		set3D();
        draw(dTime);

		set2D();
		drawUI(dTime);
        console->draw(dTime);
        
        glfwSwapBuffers();
    }
}

void TriumphGame::getWindowSize(int *w, int *h)
{
    *w = m_windowWidth;
    *h = m_windowHeight;
}

float TriumphGame::getElapsedTime()
{
    return m_elapsedTime;
}

void TriumphGame::clean()
{
    glDeleteLists(m_meshGlobe, 1);
    delete m_texGlobe;
    
    glfwTerminate();
    
    // kill singleton
    delete m_pInstance;
}

