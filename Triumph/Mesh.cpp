//
//  Mesh.cpp
//  Triumph
//
//  Created by Justin Labak on 11/28/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "Mesh.h"
#include "GameEngine.h"
#include "VertMods.h"

#include <math.h>

// GPU has VBO at this point
Mesh::Mesh() {
    
    m_memHint = GL_STATIC_DRAW_ARB;
    m_vertMod = NULL;

	m_pWVertices = NULL;
    
    // Get Pointers To The GL Functions
    glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC) glfwGetProcAddress("glGenBuffersARB");
    glBindBufferARB    = (PFNGLBINDBUFFERARBPROC) glfwGetProcAddress("glBindBufferARB");
    glBufferDataARB    = (PFNGLBUFFERDATAARBPROC) glfwGetProcAddress("glBufferDataARB");
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) glfwGetProcAddress("glDeleteBuffersARB");
	glMapBufferARB     = (PFNGLMAPBUFFERARBPROC) glfwGetProcAddress("glMapBufferARB");
	glUnmapBufferARB   = (PFNGLUNMAPBUFFERARBPROC) glfwGetProcAddress("glUnmapBufferARB");
}

void Mesh::setMemHint(int hint) {
    m_memHint = hint;
}

void Mesh::setVertMod(VertMod *mod) {
    m_vertMod = mod;
}

Mesh::~Mesh() {
    delete [] m_pVertices; m_pVertices = NULL;
    delete [] m_pTexCoords; m_pTexCoords = NULL;
    
	delete[] m_pWVertices; m_pWVertices = NULL;

	glDeleteBuffersARB(1, &m_nVBOVertices);
}

void Mesh::updateVertices(float* dstVertices, Vert *srcVertices, Vert *srcNormals, int count) {	
	if(!dstVertices || !srcVertices || !srcNormals)
        return;

    float height;
    float x, y, z;
    
    float time = GameEngine::getInstance()->getElapsedTime();
    
    for(int i=0; i < count; ++i) {
        // get source from original vertex array
		x = srcVertices->x;
        y = srcVertices->y;
        z = srcVertices->z;
		++srcVertices;
        
        height = m_vertMod->mod(time, x, y, z);
        
        // update vertex coords
		/*
         *dstVertices = x + (height * *srcNormals);  // x
         ++dstVertices; ++srcNormals;
         *dstVertices = y + (height * *srcNormals);  // y
         ++dstVertices; ++srcNormals;
         *dstVertices = z + (height * *srcNormals);  // z
         ++dstVertices; ++srcNormals;
         */
		//*dstVertices = x;
		++dstVertices;
		*dstVertices = y + height;
		++dstVertices;
		//*dstVertices = z;
		++dstVertices;
    }
}

void Mesh::draw(float dTime) {
    
    // Enable Pointers
	glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
    
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
    
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );

        // map the buffer object into client's memory
        // Note that glMapBufferARB() causes sync issue.
        // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
        // for GPU to finish its job.
        if (m_vertMod != NULL) {
            float *ptr = (float *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
            if(ptr)
            {
                // wobble vertex in and out along normal
                m_vertMod->update(dTime);
                updateVertices(ptr, m_pVertices, m_pVertices, m_nVertexCount);
                glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // release pointer to mapping buffer
            }
        }

        glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
        glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
        
    } else {
        
        if (m_vertMod != NULL) {
            m_vertMod->update(dTime);
            updateVertices((float*)m_pWVertices, m_pVertices, m_pVertices, m_nVertexCount);
        }
        glVertexPointer( 3, GL_FLOAT, 0, m_pWVertices ); // Set The Vertex Pointer To Our Vertex Data
        glTexCoordPointer( 2, GL_FLOAT, 0, m_pTexCoords );  // Set The Vertex Pointer To Our TexCoord Data
    
    }
    
    // Render
    glEnable(GL_TEXTURE_2D);
    glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount );        // Draw All Of The Triangles At Once
    glDisable(GL_TEXTURE_2D);

    // Disable Pointers
    glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
    
    // release VBOs with ID 0
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }

}

void Mesh::buildVBOs() {
    
    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );                  // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );         // Bind The Buffer
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*3*sizeof(float), m_pVertices, m_memHint );

    // Generate And Bind The Texture Coordinate Buffer
    glGenBuffersARB( 1, &m_nVBOTexCoords );                 // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );        // Bind The Buffer
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*2*sizeof(float), m_pTexCoords, GL_STATIC_DRAW_ARB );

}


bool Mesh::loadHeightmap( const char* szPath, float flHeightScale, float flResolution ) {
    int sizeX, sizeY;
    
    // Load Texture Data
    m_pTextureImage = Texture::CreateFromFile(szPath);
    m_pTextureImage->size(&sizeX, &sizeY);
    
    // Generate Vertex Field
    m_nVertexCount = (int) ( sizeX * sizeY * 6 / ( flResolution * flResolution ) );
    m_pVertices = new Vert[m_nVertexCount];                 // Allocate Vertex Data
    m_pTexCoords = new TexCoord[m_nVertexCount];               // Allocate Tex Coord Data
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
                m_pVertices[nIndex].y = m_pTextureImage->height( (int) flX, (int) flZ ) *  flHeightScale;
                m_pVertices[nIndex].z = flZ - ( sizeY / 2 );
                
                // Stretch The Texture Across The Entire Mesh
                m_pTexCoords[nIndex].u = flX / sizeX;
                m_pTexCoords[nIndex].v = flZ / sizeY;
                
                // Increment Our Index
                nIndex++;
            }
            // fix normals
            Vert v = m_pVertices[nIndex-4];
            m_pVertices[nIndex-4] = m_pVertices[nIndex-5];
            m_pVertices[nIndex-5] = v;
        }
    }
    
    // Load Vertex Data Into The Graphics Card Memory
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
        buildVBOs();
    } else {
		m_pWVertices = new Vert[m_nVertexCount];
		memcpy(m_pWVertices, m_pVertices, m_nVertexCount * sizeof(Vert));
	}
    
    return true;
}