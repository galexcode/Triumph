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


TriumphGame::TriumphGame()
{
    Console::getInstance()->message(CONSOLE_MSG_SYS, "Starting Triumph");

    m_engine = GameEngine::getInstance();
	m_meshGlobeZoom = 0;
}

int TriumphGame::init()
{
    // init the debug font
	m_font = new Font("courier.bmp");
	m_font->init();

    // init the camera location
	m_cam.m_eye = Vector3(0, 0, -3000.0f);
    
    initGlobe();
    initSkybox();
    
    return INIT_SUCCESS;
}

void TriumphGame::mouseButtonEvent(int button, int state)
{
	switch (button)
	{
		case INPUTBTN_MOUSELEFT:
		{
            if (!m_engine->m_input->m_fMouseLeft)
            {
                m_dragGlobeStart = Vector3::zero;
            }
		}
	}
}

void TriumphGame::mouseWheelEvent(int dir)
{
	m_meshGlobeZoom += dir;
    if (m_meshGlobeZoom < 1)
        m_meshGlobeZoom = 1;
    Vector3 eyeNorm = m_cam.m_eye;
    eyeNorm.normalize();
	m_cam.m_eye += (dir) * eyeNorm * -10;
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
    
    m_globe = new GameObject();
	m_globe->rotate(Vector3::right, PI / 2);
    m_globe->m_boundingRadius = radius;
}

void TriumphGame::initSkybox()
{
    m_texSkybox[TOP_ID] = Texture::CreateFromFile("skyboxhigh_top.bmp");
    m_texSkybox[BOTTOM_ID] = Texture::CreateFromFile("skyboxhigh_bottom.bmp");
    m_texSkybox[LEFT_ID] = Texture::CreateFromFile("skyboxhigh_left.bmp");
    m_texSkybox[RIGHT_ID] = Texture::CreateFromFile("skyboxhigh_right.bmp");
    m_texSkybox[FRONT_ID] = Texture::CreateFromFile("skyboxhigh_front.bmp");
    m_texSkybox[BACK_ID] = Texture::CreateFromFile("skyboxhigh_back.bmp");
}

void TriumphGame::drawUI(float dTime)
{
	char buf[256];
	sprintf(buf, "%d", (int)m_engine->m_fps);
	m_font->print(buf, m_engine->m_windowWidth - 30, m_engine->m_windowHeight - 20);
}

// use this function to get the camera positioned properly
void TriumphGame::updateCamera()
{
    //glTranslatef(0, 0, -3000 + m_meshGlobeZoom * 10);
	
    // camera is based on a 3d system, so make sure the 3d
    // perspective is set before the lookat transform is done
    m_engine->set3D();
    
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
    glBindTexture(GL_TEXTURE_2D, m_texGlobe->gid());
    glCallList(m_meshGlobe);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    
    drawSkybox(dTime);
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
    gluUnProject(m_engine->m_input->m_cursorX,
                 m_engine->m_windowHeight - m_engine->m_input->m_cursorY,
                 0.0f,
                 mvMatrix,
                 projMatrix,
                 viewport,
                 &m1x,&m1y,&m1z);
    gluUnProject(m_engine->m_input->m_cursorX,
                 m_engine->m_windowHeight - m_engine->m_input->m_cursorY,
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
    updateCamera();
    
    float angle = 0;
    Vector3 vec;
    Vector3 dir = m_cam.m_target - m_cam.m_eye;
    if (m_engine->m_input->m_fLeft || m_engine->m_input->m_fRight)
    {
        if (m_engine->m_input->m_fLeft)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        if (m_engine->m_input->m_fRight)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = Vector3::up;
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    if (m_engine->m_input->m_fUp || m_engine->m_input->m_fDown)
    {
        if (m_engine->m_input->m_fUp)
        {
            angle = -0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        if (m_engine->m_input->m_fDown)
        {
            angle += 0.02f; // / m_meshGlobeZoom;
            vec = dir.cross(m_cam.m_up);
        }
        m_cam.m_eye = Quaternion::CreateFromAxis(vec, angle) * m_cam.m_eye;
    }
    
    if (m_engine->m_input->m_fMouseLeft)
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

}

void TriumphGame::drawSkybox(float dTime)
{
    /*
     All this function does is create 6 squares and distance them according to the measurements
     provided by the user. Mess around with it if you want.
     */
    
    
    glEnable(GL_TEXTURE_2D);
    
    // Bind the BACK texture of the sky map to the BACK side of the cube
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[BACK_ID]->gid());
    
    float size = 10000.0f;
    float width, height, length;
    width = height = length = size;
    
    // Center the skybox
    float x = m_cam.m_eye.m_x - width / 2;
    float y = m_cam.m_eye.m_y - height / 2;
    float z = m_cam.m_eye.m_z - length / 2;
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[FRONT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[BOTTOM_ID]->gid());
    glBegin(GL_QUADS);
    
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[TOP_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[RIGHT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, m_texSkybox[LEFT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void TriumphGame::clean()
{
    glDeleteLists(m_meshGlobe, 1);
    
    delete m_texGlobe;
    delete m_texSkybox[TOP_ID];
    delete m_texSkybox[BOTTOM_ID];
    delete m_texSkybox[LEFT_ID];
    delete m_texSkybox[RIGHT_ID];
    delete m_texSkybox[FRONT_ID];
    delete m_texSkybox[BACK_ID];
    
    delete m_globe;
}

