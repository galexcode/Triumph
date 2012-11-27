//
//  BattleScene.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__BattleScene__
#define __Triumph__BattleScene__

#include "Scene.h"
#include "Texture.h"
#include "GameCamera.h"
#include "GameEngine.h"

class BattleScene : public Scene
{
private:
    GameCamera m_cam;
    GameEngine *m_engine;

	bool m_fFreeLook;

public:
	int load();
    
    void update(float dTime);
    void draw(float dTime);

	void keyEvent(int key, int state);

};



class CVert                                 // Vertex Class
{
public:
    float x;                                // X Component
    float y;                                // Y Component
    float z;                                // Z Component
};
typedef CVert CVec;                             // The Definitions Are Synonymous

class CTexCoord                                 // Texture Coordinate Class
{
public:
    float u;                                // U Component
    float v;                                // V Component
};

class CMesh
{
public:
    // Mesh Data
    int     m_nVertexCount;                     // Vertex Count
    CVert*      m_pVertices;                        // Vertex Data
    CTexCoord*  m_pTexCoords;                       // Texture Coordinates
    unsigned int    m_nTextureId;                       // Texture ID
    
    // Vertex Buffer Object Names
    unsigned int    m_nVBOVertices;                     // Vertex VBO Name
    unsigned int    m_nVBOTexCoords;                    // Texture Coordinate VBO Name
    
    // Temporary Data
    Texture *m_pTextureImage;                   // Heightmap Data
    
public:
    //CMesh();                                // Mesh Constructor
    //~CMesh();                               // Mesh Deconstructor
    
    // Heightmap Loader
    bool LoadHeightmap( const char* szPath, float flHeightScale, float flResolution );
    // Single Point Height
    float PtHeight( int nX, int nY );
    // VBO Build Function
    void BuildVBOs();
};

#endif /* defined(__Triumph__BattleScene__) */
