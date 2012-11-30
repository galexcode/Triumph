//
//  Mesh.h
//  Triumph
//
//  Created by Justin Labak on 11/28/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__Mesh__
#define __Triumph__Mesh__

#include "Gfx.h"
#include "Texture.h"
#include "Geometry.h"
#include "VertMods.h"

typedef struct
{
	float x;
	float y;
	float z;
} Vert;

class TexCoord                           
{
public:
    float u;
    float v;
};

class Mesh
{
private:
    
    int m_memHint;
    VertModFunc m_vertModFunc;
    
    // Mesh Data
    int     m_nVertexCount; // Vertex Count
    Vert*      m_pVertices; // Vertex Data
	Vert*	   m_pWVertices; // Writable Vertex Data when VBO not available
    TexCoord*  m_pTexCoords; // Texture Coordinates
    unsigned int    m_nTextureId; // Texture ID
    
    // Vertex Buffer Object Names
    unsigned int    m_nVBOVertices; // Vertex VBO Name
    unsigned int    m_nVBOTexCoords; // Texture Coordinate VBO Name
    
    // Temporary Data
    Texture *m_pTextureImage; // Heightmap Data
    
    // Pointers To The GL VBO Functions
    PFNGLGENBUFFERSARBPROC glGenBuffersARB;
    PFNGLBINDBUFFERARBPROC glBindBufferARB;
    PFNGLBUFFERDATAARBPROC glBufferDataARB;
    PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
	PFNGLMAPBUFFERARBPROC glMapBufferARB;
	PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
    
    void updateVertices(float* dstVertices, Vert *srcVertices, Vert *srcNormals, int count);
    
public:
    Mesh();
    ~Mesh();
    
    void setMemHint(int hint);
    void setVertModFunc(int mod);
    
    bool loadHeightmap( const char* szPath, float flHeightScale, float flResolution );
    
    void buildVBOs();
    
    void draw(float dTime);
};

#endif /* defined(__Triumph__Mesh__) */
