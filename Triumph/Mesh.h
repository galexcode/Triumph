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
#include <vector>

typedef struct
{
	float x;
	float y;
	float z;
    
    float nx;
    float ny;
    float nz;
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
    VertMod *m_vertMod;
    
    // Shader source
    GLuint m_shaderProgram;
    
    GLuint m_attrPosition;
    GLuint m_attrNormal;
    
    // Mesh Data
    int             m_nVertexCount;
    int             m_nIndexCount;
    
    Vert            *m_pVertices; // Vertex Data
    unsigned short  *m_pIndices; // Index Data
    
	Vert            *m_pWVertices; // Writable Vertex Data when VBO not available
    
    TexCoord        *m_pTexCoords; // Texture Coordinates
    unsigned int    m_nTextureId; // Texture ID
    
    // Vertex Buffer Object Names
    unsigned int    m_nVBOVertices; // Vertex VBO Name
    unsigned int    m_nVBOTexCoords; // Texture Coordinate VBO Name
    unsigned int    m_idVBOIndices;
    
    // Temporary Data
    Texture *m_pTextureImage; // Heightmap Data
    
    void updateVertices(float* dstVertices, Vert *srcVertices, Vert *srcNormals, int count);
    
    void buildVBOs();
    
    char * loadTxtSource(const char *source);
    
    //void setShader(char *source, GLenum *shader, GLenum type);
    
public:
    Mesh();
    ~Mesh();
    
    void setMemHint(int hint);
    void setVertMod(VertMod *mod);
    void setShaders(const char **files, const GLenum *types, int nShaders);
    
    bool loadHeightmap( const char* szPath, float flHeightScale, float flResolution );
    
    void draw(float dTime);
};

#endif /* defined(__Triumph__Mesh__) */
