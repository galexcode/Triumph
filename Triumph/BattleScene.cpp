//
//  BattleScene.cpp
//  Triumph
//
//  Created by Justin Labak on 11/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include <GL/glew.h>
#include <GL/glfw.h>
#include "GLUtil.h"

#include "BattleScene.h"
#include "Console.h"
#include <cstdio>
#include <cstring>
#include "GameEngine.h"
#include "GameInput.h"

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4

#define MESH_RESOLUTION 4.0f
#define MESH_HEIGHTSCALE 1.0f

// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;                  // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;                  // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;                  // VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;            // VBO Deletion Procedure

bool        g_fVBOSupported = false;                // ARB_vertex_buffer_object supported?
CMesh*      g_pMesh = NULL;                         // Mesh Data
float       g_flYRot = 0.0f;                        // Rotation

int BattleScene::load() {
    
	m_engine = GameEngine::getInstance();
	m_engine->registerScene(this);

    m_cam.m_eye = Vector3(0, 0, -2);
    
    // Load The Mesh Data
    g_pMesh = new CMesh();                          // Instantiate Our Mesh
    if( !g_pMesh->LoadHeightmap( "terrain.bmp",              // Load Our Heightmap
                                MESH_HEIGHTSCALE, MESH_RESOLUTION ) )
    {
        Console::getInstance()->message(0, "Error Loading Heightmap");
        return false;
    }
    
    // Check For VBOs Supported
#ifndef NO_VBOS
    g_fVBOSupported = (bool)glfwExtensionSupported( "GL_ARB_vertex_buffer_object" );
    if( g_fVBOSupported )
    {
        // Get Pointers To The GL Functions
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) glfwGetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC) glfwGetProcAddress("glBindBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC) glfwGetProcAddress("glBufferDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) glfwGetProcAddress("glDeleteBuffersARB");
        // Load Vertex Data Into The Graphics Card Memory
        g_pMesh->BuildVBOs();                        // Build The VBOs
    }
#else /* NO_VBOS */
    g_fVBOSupported = false;
#endif
    
    
    return 0;
}

void BattleScene::draw(float dTime) {

	if (m_engine->getDebug() == GameEngine::Mode::debug) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		GLUtil::DrawCube();
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
    
    if( !g_fVBOSupported )
    {
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_pMesh->m_nVBOVertices );
        glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_pMesh->m_nVBOTexCoords );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
    } else
    {
        glVertexPointer( 3, GL_FLOAT, 0, g_pMesh->m_pVertices ); // Set The Vertex Pointer To Our Vertex Data
        glTexCoordPointer( 2, GL_FLOAT, 0, g_pMesh->m_pTexCoords );  // Set The Vertex Pointer To Our TexCoord Data
    }
    
    // Render
    glDrawArrays( GL_TRIANGLES, 0, g_pMesh->m_nVertexCount );        // Draw All Of The Triangles At Once

    // Disable Pointers
    glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
    
}

void BattleScene::keyEvent(int key, int state) {
	if (key == INPUTKEY_SPACE && state == INPUTSTATE_RELEASE)
		m_fFreeLook = !m_fFreeLook;

	Console::getInstance()->message(0, "%d %d", m_engine->m_input->m_fUp, m_engine->m_input->m_fDown);
}

void BattleScene::update(float dTime) {

	if (m_fFreeLook) {
		if (m_engine->m_input->m_fLeft) {
			m_cam.m_eye += Vector3::right * dTime;
			m_cam.m_target += Vector3::right * dTime;
		}
		if (m_engine->m_input->m_fRight) {
			m_cam.m_eye -= Vector3::right * dTime;
			m_cam.m_target -= Vector3::right * dTime;
		}
		if (m_engine->m_input->m_fUp) {
			m_cam.m_eye -= Vector3::forward * dTime;
			m_cam.m_target -= Vector3::forward * dTime;
		}
		if (m_engine->m_input->m_fDown) {
			m_cam.m_eye += Vector3::forward * dTime;
			m_cam.m_target += Vector3::forward * dTime;
		}
	}

    GameEngine::getInstance()->set3D();
    
	gluLookAt(m_cam.m_eye.m_x, m_cam.m_eye.m_y, m_cam.m_eye.m_z,
              m_cam.m_target.m_x, m_cam.m_target.m_y, m_cam.m_target.m_z,
              m_cam.m_up.m_x, m_cam.m_up.m_y, m_cam.m_up.m_z);
    
    // Move The Camera
    //glTranslatef( 0.0f, -220.0f, 0.0f );                    // Move Above The Terrain
    //glRotatef( 10.0f, 1.0f, 0.0f, 0.0f );                   // Look Down Slightly
    //glRotatef( g_flYRot, 0.0f, 1.0f, 0.0f );                // Rotate The Camera
    
    static float milliseconds = 0;
    milliseconds += dTime;
    g_flYRot += (float) ( milliseconds ) / 1000.0f * 25.0f;
}

float CMesh :: PtHeight( int nX, int nY )
{
    GLubyte *image = m_pTextureImage->image();
    int sizeX, sizeY;
    

    
    m_pTextureImage->size(&sizeX, &sizeY);
    
    // Calculate The Position In The Texture, Careful Not To Overflow
    int nPos = ( ( nX % sizeX )  + ( ( nY % sizeY ) * sizeX ) ) * 3;
    
    //image[nPos] = 1;
    //image[nPos + 1] = 1;
    //image[nPos + 2] = 1;
    
    float flR = (float) image[ nPos ];           // Get The Red Component
    float flG = (float) image[ nPos + 1 ];           // Get The Green Component
    float flB = (float) image[ nPos + 2 ];           // Get The Blue Component
    return ( 0.299f * flR + 0.587f * flG + 0.114f * flB );          // Calculate The Height Using The Luminance Algorithm
}

void CMesh :: BuildVBOs()
{
    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );                  // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );         // Bind The Buffer
    // Load The Data
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*3*sizeof(float), m_pVertices, GL_STATIC_DRAW_ARB );
    
    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB( 1, &m_nVBOTexCoords );                 // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );        // Bind The Buffer
    // Load The Data
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*2*sizeof(float), m_pTexCoords, GL_STATIC_DRAW_ARB );
    
    // Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
    delete [] m_pVertices; m_pVertices = NULL;
    delete [] m_pTexCoords; m_pTexCoords = NULL;
}


bool CMesh :: LoadHeightmap( const char* szPath, float flHeightScale, float flResolution )
{
    int sizeX, sizeY;
    
    // Load Texture Data
    //m_pTextureImage = auxDIBImageLoad( szPath );                // Utilize GLaux's Bitmap Load Routine
    m_pTextureImage = Texture::CreateFromFile(szPath);
 
    m_pTextureImage->size(&sizeX, &sizeY);
    
    // Generate Vertex Field
    m_nVertexCount = (int) ( sizeX * sizeY * 6 / ( flResolution * flResolution ) );
    m_pVertices = new CVec[m_nVertexCount];                 // Allocate Vertex Data
    m_pTexCoords = new CTexCoord[m_nVertexCount];               // Allocate Tex Coord Data
    int nX, nZ, nTri, nIndex=0;                     // Create Variables
    float flX, flZ;
    for( nZ = 0; nZ < sizeY; nZ += (int) flResolution )
    {
        for( nX = 0; nX < sizeX; nX += (int) flResolution )
        {
            for( nTri = 0; nTri < 6; nTri++ )
            {
                // Using This Quick Hack, Figure The X,Z Position Of The Point
                flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? flResolution : 0.0f );
                flZ = (float) nZ + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? flResolution : 0.0f );
                
                // Set The Data, Using PtHeight To Obtain The Y Value
                m_pVertices[nIndex].x = flX - ( sizeX / 2 );
                m_pVertices[nIndex].y = PtHeight( (int) flX, (int) flZ ) *  flHeightScale;
                m_pVertices[nIndex].z = flZ - ( sizeY / 2 );
                
                // Stretch The Texture Across The Entire Mesh
                m_pTexCoords[nIndex].u = flX / sizeX;
                m_pTexCoords[nIndex].v = flZ / sizeY;
                
                // Increment Our Index
                nIndex++;
            }
        }
    }
    
    // Load The Texture Into OpenGL // already handled by Texture class
    /*
    glGenTextures( 1, &m_nTextureId );                  // Get An Open ID
    glBindTexture( GL_TEXTURE_2D, m_nTextureId );               // Bind The Texture
    glTexImage2D( GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pTextureImage->image() );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     */
    
    // Free The Texture Data
    //delete m_pTextureImage;
    
    return true;
}