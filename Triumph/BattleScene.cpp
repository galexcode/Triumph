//
//  BattleScene.cpp
//  Triumph
//
//  Created by Justin Labak on 11/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "Gfx.h"
#include "GLUtil.h"

#include "BattleScene.h"
#include "Console.h"
#include "GameInput.h"

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4

#define MESH_RESOLUTION 16.0f
#define MESH_HEIGHTSCALE 1.0f

int BattleScene::load() {
    
	m_engine = GameEngine::getInstance();
	m_engine->registerScene(this);

    m_cam.m_eye = Vector3(0, 250, -2);
    m_cam.m_target = m_cam.m_eye + Vector3::forward;
    
    m_fFreeLook = false;
    
    // Load The Mesh Data
    m_water = new Mesh();
    m_water->setMemHint(GL_STREAM_DRAW_ARB); // make sure GL knows that this will update every frame
    m_water->setVertMod(new VertMod(FUNC_WAVES));
    
    if( !m_water->loadHeightmap( "flat1024.bmp",              // Load Our Heightmap
                                MESH_HEIGHTSCALE, MESH_RESOLUTION ) ) {
        Console::getInstance()->message(0, "Error Loading Water mesh.");
        return INIT_FAIL;
    }
    
    return INIT_SUCCESS;
}

void BattleScene::clean() {
    delete m_water;
}

void BattleScene::draw(float dTime) {

	if (m_engine->getDebug() == GameEngine::Mode::debug) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPushMatrix();
        glScalef(1000, 1000, 1000);
		GLUtil::DrawCube();
        glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_water->draw(dTime);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BattleScene::keyEvent(int key, int state) {
	if (key == INPUTKEY_SPACE && state == INPUTSTATE_RELEASE) {
		m_fFreeLook = !m_fFreeLook;
		if (m_fFreeLook) {
			glfwDisable(GLFW_MOUSE_CURSOR);
        } else {
            glfwEnable(GLFW_MOUSE_CURSOR);
        }
	}
}

void BattleScene::update(float dTime) {

    GameEngine::getInstance()->set3D();
    
	gluLookAt(m_cam.m_eye.m_x, m_cam.m_eye.m_y, m_cam.m_eye.m_z,
              m_cam.m_target.m_x, m_cam.m_target.m_y, m_cam.m_target.m_z,
              m_cam.m_up.m_x, m_cam.m_up.m_y, m_cam.m_up.m_z);

	if (m_fFreeLook) {
        int speed = 100;
		Vector3 forward = m_cam.m_eye - m_cam.m_target;
        forward.normalize();
        Vector3 right = forward.cross(m_cam.m_up);
		right.normalize();
		Vector3 up = m_cam.m_up;
		
		if (m_engine->m_input->m_fLeft) {
			m_cam.m_eye += right * dTime * speed;
			m_cam.m_target += right * dTime * speed;
		}
		if (m_engine->m_input->m_fRight) {
			m_cam.m_eye -= right * dTime * speed;
			m_cam.m_target -= right * dTime * speed;
		}
		if (m_engine->m_input->m_fUp) {
			m_cam.m_eye -= forward * dTime * speed;
			m_cam.m_target -= forward * dTime * speed;
		}
		if (m_engine->m_input->m_fDown) {
			m_cam.m_eye += forward * dTime * speed;
			m_cam.m_target += forward * dTime * speed;
		}
        
        float yaw = (m_engine->m_input->m_lastCursorX - m_engine->m_input->m_cursorX) / 100.0f;
        float pitch = (m_engine->m_input->m_cursorY - m_engine->m_input->m_lastCursorY) / 100.0f;
        Quaternion qy = Quaternion::CreateFromAxis(up, yaw);
        Quaternion qp = Quaternion::CreateFromAxis(right, pitch);
        
        m_cam.m_target = m_cam.m_eye - qy * qp * (m_cam.m_eye - m_cam.m_target);
	}
}