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
    m_fLeftDown = false;
    m_fRightDown = false;
    m_fUpDown = false;
    m_fDownDown = false;
	m_lastWheel = 0;
	m_meshGlobeZoom = 0;
}

int TriumphGame::init()
{
    
    if (!glfwInit())
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to Initialize GLFW");
        return INIT_FAIL;
    }
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, FSAA_SAMPLES);
    
    if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 8, 8, GLFW_WINDOW))
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to open GLFW window");
        return INIT_FAIL;
    }
    
    if (!glfwExtensionSupported("GL_ARB_multisample"))
    {
        Console::getInstance()->message(CONSOLE_MSG_SYS, "Context reports GL_ARB_multisample is not supported");
        return INIT_FAIL;
    }
    
    // init GLFW settings
    glfwSetWindowTitle(WINDOW_TITLE);
    glfwSetWindowSizeCallback(window_size_callback);
    glfwSetMousePosCallback(mouse_position_callback);
    glfwSetKeyCallback(keyboard_callback);
	glfwSetMouseButtonCallback(mouse_button_callback);
	glfwSetMouseWheelCallback(mouse_wheel_callback);
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
	glEnable (GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
    
    // init the visual console
    Console::getInstance()->initScreenDisplay();

    // init the debug font
	m_font = new Font("courier.bmp");
	m_font->init();

    // init the camera location
	m_cam.m_eye = Vector3(0, 0, -3000.0f);
    
    initGlobe();
    initSkybox();
    
    
    return INIT_SUCCESS;
}

void TriumphGame::windowResize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    
    glViewport(0, 0, m_windowWidth, m_windowHeight);
}

void TriumphGame::mouseMove(int x, int y)
{
    m_cursorX = x;
    m_cursorY = y;
}

void TriumphGame::keyEvent(int key, int state)
{
    if (key == (int)'A')
        m_fLeftDown = !m_fLeftDown;
    else if (key == (int)'D')
        m_fRightDown = !m_fRightDown;
    else if (key == (int)'W')
        m_fUpDown = !m_fUpDown;
    else if (key == (int)'S')
        m_fDownDown = !m_fDownDown;
}

void TriumphGame::mouseButtonEvent(int button, int state)
{
	switch (button)
	{
		case INPUTBTN_MOUSELEFT:
		{
			m_fLeftMouseDown = state == INPUTSTATE_PRESS;
            if (!m_fLeftMouseDown)
            {
                m_dragGlobeStart = Vector3::zero;
            }
		}
	}
}

void TriumphGame::mouseWheelEvent(int pos)
{
	m_meshGlobeZoom += pos - m_lastWheel;
    if (m_meshGlobeZoom < 1)
        m_meshGlobeZoom = 1;
    Vector3 eyeNorm = m_cam.m_eye;
    eyeNorm.normalize();
	m_cam.m_eye += (pos - m_lastWheel) * eyeNorm * -10;
    m_lastWheel = pos;
    m_dragGlobeStart = Vector3::zero;
}

void TriumphGame::initGlobe()
{
    float radius = 1000.0f;
    m_texGlobe = Texture::CreateFromFile("earth_hires.bmp");
    
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
    m_meshGlobe = glGenLists(1);
    glNewList(m_meshGlobe, GL_COMPILE);
    gluSphere(sphere, radius, 200, 200);
    glEndList();
    gluDeleteQuadric(sphere);
	m_meshGlobeRot = 0;
    
    m_globe = new GameObject();
	m_globe->rotate(Vector3::right, PI / 2);
    m_globe->m_boundingRadius = radius;
}

void TriumphGame::initSkybox()
{
    float radius = 4000.0f;
    m_texSkybox = Texture::CreateFromFile("sky2.bmp");
    
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricOrientation(sphere, GLU_INSIDE);
    m_meshSkybox = glGenLists(1);
    glNewList(m_meshSkybox, GL_COMPILE);
    gluSphere(sphere, radius, 200, 200);
    glEndList();
    gluDeleteQuadric(sphere);
    
    m_skybox = new GameObject();
    m_skybox->m_boundingRadius = radius;
}

void TriumphGame::drawUI(float dTime)
{
	char buf[256];
	sprintf(buf, "%d", (int)m_fps);
	m_font->print(buf, m_windowWidth - 30, m_windowHeight - 20);
}

// use this function to get the camera positioned properly
void TriumphGame::updateCamera()
{
    //glTranslatef(0, 0, -3000 + m_meshGlobeZoom * 10);
	
    // camera is based on a 3d system, so make sure the 3d
    // perspective is set before the lookat transform is done
    set3D();
    
	gluLookAt(m_cam.m_eye.m_x, m_cam.m_eye.m_y, m_cam.m_eye.m_z,
		m_cam.m_target.m_x, m_cam.m_target.m_y, m_cam.m_target.m_z,
		m_cam.m_up.m_x, m_cam.m_up.m_y, m_cam.m_up.m_z);
}

void TriumphGame::draw(float dTime)
{
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(m_globe->m_position.m_x, m_globe->m_position.m_y, m_globe->m_position.m_z);
    glMultMatrixf(m_globe->m_rotation.getMatrix().v);
    glRotatef(m_meshGlobeRot, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, m_texGlobe->gid());
    glCallList(m_meshGlobe);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(m_cam.m_eye.m_x, m_cam.m_eye.m_y, m_cam.m_eye.m_z);
    glBindTexture(GL_TEXTURE_2D, m_texSkybox->gid());
    glCallList(m_meshSkybox);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

Ray3 TriumphGame::getMouseRay()
{
    GLdouble m1x,m1y,m1z,m2x,m2y,m2z;
    Vector3 ray_dir;
    Vector3 ray_pos;
    
    GLint viewport[4];
    GLdouble projMatrix[16], mvMatrix[16];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    
    //unproject to find actual coordinates
    gluUnProject(m_cursorX,
                 m_windowHeight - m_cursorY,
                 0.0f,
                 mvMatrix,
                 projMatrix,
                 viewport,
                 &m1x,&m1y,&m1z);
    gluUnProject(m_cursorX,
                 m_windowHeight - m_cursorY,
                 1.0f,
                 mvMatrix,
                 projMatrix,
                 viewport,
                 &m2x,&m2y,&m2z);
    
    ray_pos = Vector3(m1x, m1y, m1z);
    ray_dir = Vector3(m2x, m2y, m2z) - ray_pos;
    ray_dir.normalize(); // maybe not necessary to have a unit direction
    return Ray3(ray_pos, ray_dir);
}

// the camera rotation parts of this function
// require that 3d mode and the camera be fully
// prepared for the scene
void TriumphGame::update(float dTime)
{
    float angle = 0;
    Vector3 vec;
    Vector3 dir = m_cam.m_target - m_cam.m_eye;
    if (m_fLeftDown || m_fRightDown)
    {
        if (m_fLeftDown)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        if (m_fRightDown)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    if (m_fUpDown || m_fDownDown)
    {
        if (m_fUpDown)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        if (m_fDownDown)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    
    if (m_fLeftMouseDown)
	{
        // this seems pretty sloppy, but it will allow us to
        // rotate the camera about the mouse drag of the globe
        // todo: clean up, perhaps optimize
        Ray3 ray = getMouseRay();
        Sphere globeBounds(m_globe->m_position, m_globe->m_boundingRadius);
        Vector3 hit;
        if (ray.intersects(globeBounds, &hit))
        {
            if (m_dragGlobeStart.equals(Vector3::zero))
            {
                m_dragGlobeStart = hit;
            }
            else
            {
                float angle = m_dragGlobeStart.angleBetween(hit);
                if (angle != 0)
                {
                    Quaternion q = Quaternion::CreateFromAxis(hit.cross(m_dragGlobeStart),
                                                              angle);
                    m_cam.m_eye = q * m_cam.m_eye;
                    
                    updateCamera();
                    ray = getMouseRay();
                    ray.intersects(globeBounds, &hit);
                    m_dragGlobeStart = hit;
                }
            }
        }
	}
    
    
	m_lastCursorX = m_cursorX;
	m_lastCursorY = m_cursorY;
}

void TriumphGame::set3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, m_windowWidth / m_windowHeight, 0.1, 10000);

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
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		updateCamera(); // also sets 3d mode
        
        update(dTime);
        console->update(dTime);
        
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
    glDeleteLists(m_meshSkybox, 1);
    delete m_texGlobe;
    delete m_texSkybox;
    
    glfwTerminate();
    
    // kill singleton
    delete m_pInstance;
}

