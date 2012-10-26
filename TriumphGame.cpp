//
//  TriumphGame.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "TriumphGame.h"
#include "GameInput.h"
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
    glfwSwapInterval(1);
    
    int samples = glfwGetWindowParam(GLFW_FSAA_SAMPLES);
    if (samples)
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "%s %i", "Context reports FSAA sample size is", samples);
    }
    
    Console::getInstance()->initScreenDisplay();
    
    glClearColor(0, 0, 0, 0);
    
    return INIT_SUCCESS;
}

void TriumphGame::draw(float dTime)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.f, 1.f, 1.f);
    
    glBegin(GL_LINES);
    glVertex2f(0.f, (GLfloat) m_windowHeight - m_cursorY);
    glVertex2f((GLfloat) m_windowWidth, (GLfloat) m_windowHeight - m_cursorY);
    glVertex2f((GLfloat) m_cursorX, 0.f);
    glVertex2f((GLfloat) m_cursorX, (GLfloat) m_windowHeight);
    glEnd();
    
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.f, 1.f, 0.f, 1.f);
    
    glLoadIdentity();
    glTranslatef(0.5f, 0.f, 0.f);
    glRotatef(m_elapsedTime, 0.f, 0.f, 1.f);
    
    glEnable(GL_MULTISAMPLE_ARB);
    glColor3f(1.f, 1.f, 1.f);
    glRectf(-0.25f, -0.25f, 0.25f, 0.25f);
    
    glLoadIdentity();
    glTranslatef(-0.5f, 0.f, 0.f);
    glRotatef(m_elapsedTime, 0.f, 0.f, 1.f);
    
    glDisable(GL_MULTISAMPLE_ARB);
    glColor3f(1.f, 1.f, 1.f);
    glRectf(-0.25f, -0.25f, 0.25f, 0.25f);
}

void TriumphGame::update(float dTime)
{
    
}

void TriumphGame::run()
{
    m_elapsedTime = (float) glfwGetTime();
    
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        float dTime = (float) glfwGetTime() - m_elapsedTime;
        m_elapsedTime += dTime;
        Console *console = Console::getInstance();
        
        update(dTime);
        console->update(dTime);
        
        draw(dTime);
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
    glfwTerminate();
    
    // kill singleton
    delete m_pInstance;
}

