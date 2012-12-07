//
//  GLUtil.cpp
//  Triumph
//
//  Created by Justin Labak on 11/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GLUtil.h"
#include "GameEngine.h"
#include "Console.h"

void GLUtil::DrawCube() {
    glBegin(GL_QUADS);          // Start Drawing Quads
    // Front Face
    glNormal3f( 0.0f, 0.0f, 1.0f);      // Normal Facing Forward
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);      // Normal Facing Away
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);      // Normal Facing Up
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    // Bottom Face
    glNormal3f( 0.0f,-1.0f, 0.0f);      // Normal Facing Down
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);      // Normal Facing Right
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);      // Normal Facing Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    glEnd();                    // Done Drawing Quads
}

Ray3 GLUtil::GetMouseRay()
{
    GameEngine *engine = GameEngine::getInstance();
    
    GLdouble m1x,m1y,m1z,m2x,m2y,m2z;
    Vector3 ray_dir;
    Vector3 ray_pos;
    
    GLint viewport[4];
    GLdouble projMatrix[16], mvMatrix[16];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX,mvMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
    
    //unproject to find actual coordinates
    gluUnProject(engine->m_input->m_cursorX,
                 engine->m_windowHeight - engine->m_input->m_cursorY,
                 0.0f,
                 mvMatrix,
                 projMatrix,
                 viewport,
                 &m1x,&m1y,&m1z);
    gluUnProject(engine->m_input->m_cursorX,
                 engine->m_windowHeight - engine->m_input->m_cursorY,
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

void GLUtil::DrawSkybox(Vector3 eye, Texture *textures[6]) {
    glEnable(GL_TEXTURE_2D);
    
    // Bind the BACK texture of the sky map to the BACK side of the cube
    glBindTexture(GL_TEXTURE_2D, textures[BACK_ID]->gid());
    
    float size = 10000.0f;
    float width, height, length;
    width = height = length = size;
    
    // Center the skybox
    float x = eye.m_x - width / 2;
    float y = eye.m_y - height / 2;
    float z = eye.m_z - length / 2;
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[FRONT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, textures[BOTTOM_ID]->gid());
    glBegin(GL_QUADS);
    
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[TOP_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[RIGHT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
    
    glEnd();
    glBindTexture(GL_TEXTURE_2D, textures[LEFT_ID]->gid());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


GLuint GLUtil::CreateShader(GLenum eShaderType, const char *source) {
    GLuint shader = glCreateShaderObjectARB(eShaderType);
    glShaderSourceARB(shader, 1, (const char**)&source, NULL);
    
    glCompileShaderARB(shader);
    
    // debugging
    if (true)
    {
        GLchar *strInfoLog = new GLchar[1000];
        GLsizei nChars;
        glGetInfoLogARB(shader, 999, &nChars, strInfoLog);
        strInfoLog[1000] = '\0';
        
        if (nChars != 0) {
            const char *strShaderType = NULL;
            switch(eShaderType)
            {
                case GL_VERTEX_SHADER: strShaderType = "Vertex"; break;
                case GL_GEOMETRY_SHADER: strShaderType = "Geometry"; break;
                case GL_FRAGMENT_SHADER: strShaderType = "Fragment"; break;
            }
            
            Console::getInstance()->message(CONSOLE_MSG_SYS, "%s Shader: %s", strShaderType, strInfoLog);
        }
        delete[] strInfoLog;
    }
    
	return shader;
}

GLuint GLUtil::CreateShaderProgram(GLuint *shaderList, int nShaders) {
    GLuint program = glCreateProgramObjectARB();
    
    // attach list of compiled shaders to the shader program
    for(size_t iLoop = 0; iLoop < nShaders; iLoop++)
    	glAttachObjectARB(program, shaderList[iLoop]);
    
    //glBindAttribLocationARB(*program, 0, "position");
    
    glLinkProgramARB(program);
    
    // debugging
    if (true)
    {
        GLchar *strInfoLog = new GLchar[1000];
        GLsizei nChars;
        glGetInfoLogARB(program, 999, &nChars, strInfoLog);
        strInfoLog[1000] = '\0';
        
        if (nChars != 0) {
            Console::getInstance()->message(CONSOLE_MSG_SYS, "GLSL Linker: %s", strInfoLog);
        }
        delete[] strInfoLog;
    }
    
    // the shaders are no longer needed by the program once compiled
    for(size_t iLoop = 0; iLoop < nShaders; iLoop++)
        glDetachObjectARB(program, shaderList[iLoop]);
    
    return program;
    
}