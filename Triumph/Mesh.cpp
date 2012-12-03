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
#include "Console.h"

#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

// GPU has VBO at this point
Mesh::Mesh() {
    
    m_memHint = GL_STATIC_DRAW_ARB;
    m_vertMod = NULL;
    
    m_shaderProgram = 0;

	m_pWVertices = NULL;
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

    glDeleteProgramsARB(1, &m_shaderProgram);
    
	glDeleteBuffersARB(1, &m_nVBOVertices);
}

char * Mesh::loadTxtSource(const char *source) {
    char *txt = NULL;
    
    std::fstream file(source);
    if (file.good()) {
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        txt = new char[str.size() + 1];
        std::copy(str.begin(), str.end(), txt);
        txt[str.size()] = '\0';
    }
    return txt;
}

GLuint Mesh::createShader(GLenum eShaderType, const char *source) {
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

void Mesh::createShaderProgram(GLuint *program, const std::vector<GLuint> shaderList) {
    glDeleteProgramsARB(1, program); // clean up last program
    *program = glCreateProgramObjectARB();
    
    // attach list of compiled shaders to the shader program
    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    	glAttachObjectARB(*program, shaderList[iLoop]);
    
    //glBindAttribLocationARB(*program, 0, "position");
    
    glLinkProgramARB(*program);
    
    // debugging
    if (true)
    {
        GLchar *strInfoLog = new GLchar[1000];
        GLsizei nChars;
        glGetInfoLogARB(*program, 999, &nChars, strInfoLog);
        strInfoLog[1000] = '\0';
        
        if (nChars != 0) {
            Console::getInstance()->message(CONSOLE_MSG_SYS, "GLSL Linker: %s", strInfoLog);
        }
        delete[] strInfoLog;
    }
        
    // the shaders are no longer needed by the program once compiled
    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachObjectARB(*program, shaderList[iLoop]);
    
    m_attrPosition = glGetAttribLocationARB(*program, "position");
    m_attrNormal   = glGetAttribLocationARB(*program, "normal");
}

void Mesh::setShaders(const char **files, const GLenum *types, int nShaders) {
    std::vector<GLuint> shaders;
    for (int i = 0; i < nShaders; ++i) {
        char *source = loadTxtSource(files[i]);
        shaders.push_back(createShader(types[i], source));
        delete [] source;
    }
    createShaderProgram(&m_shaderProgram, shaders);
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
    
    if (m_shaderProgram > 0)
        glUseProgramObjectARB(m_shaderProgram);
    
    
    // Enable Pointers
	//glEnableClientState( GL_VERTEX_ARRAY );						// Enable Vertex Arrays
	//glEnableClientState( GL_TEXTURE_COORD_ARRAY );				// Enable Texture Coord Arrays
    
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
    
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
        glVertexAttribPointerARB(m_attrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
        glVertexAttribPointerARB(m_attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)3);
        glEnableVertexAttribArrayARB(m_attrPosition);
        glEnableVertexAttribArrayARB(m_attrNormal);
        
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
                //updateVertices(ptr, m_pVertices, m_pVertices, m_nVertexCount);
                glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); // release pointer to mapping buffer
            }
        }
        
        //glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
        //glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
        //glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
        
    } else {
        
        if (m_vertMod != NULL) {
            m_vertMod->update(dTime);
            //updateVertices((float*)m_pWVertices, m_pVertices, m_pVertices, m_nVertexCount);
        }
        glVertexPointer( 6, GL_FLOAT, 0, m_pWVertices ); // Set The Vertex Pointer To Our Vertex Data
        //glTexCoordPointer( 2, GL_FLOAT, 0, m_pTexCoords );  // Set The Vertex Pointer To Our TexCoord Data
    
    }
    
    // Render
    //glEnable(GL_TEXTURE_2D);
    glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount );        // Draw All Of The Triangles At Once
    //glDisable(GL_TEXTURE_2D);

    // Disable Pointers
    //glDisableClientState( GL_VERTEX_ARRAY );                    // Disable Vertex Arrays
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY );             // Disable Texture Coord Arrays
    
    // release VBOs with ID 0
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
        glDisableVertexAttribArrayARB(m_attrPosition);
        glDisableVertexAttribArrayARB(m_attrNormal);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }
    
    if (m_shaderProgram > 0)
        glUseProgramObjectARB(0);

}

void Mesh::buildVBOs() {
    
    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );                  // Get A Valid Name
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );         // Bind The Buffer
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(Vert) * m_nVertexCount, m_pVertices, m_memHint );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0); // cleanup

    // Generate And Bind The Texture Coordinate Buffer
    //glGenBuffersARB( 1, &m_nVBOTexCoords );                 // Get A Valid Name
    //glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );        // Bind The Buffer
    //glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount * sizeof(TexCoord), m_pTexCoords, GL_STATIC_DRAW_ARB );
    //glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0); // cleanup

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
                
                // normals point straight up for testing
                m_pVertices[nIndex].nx = 0;
                m_pVertices[nIndex].ny = 1;
                m_pVertices[nIndex].nz = 0;
                
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