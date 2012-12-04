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

    delete[] m_pIndices; m_pIndices = NULL;
    
    glDeleteProgramsARB(1, &m_shaderProgram);
    
	glDeleteBuffersARB(1, &m_nVBOVertices);
    glDeleteBuffersARB(1, &m_idVBOIndices);
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
    
    m_attrPosition = glGetAttribLocationARB(*program, "position_in");
    m_attrNormal   = glGetAttribLocationARB(*program, "normal_in");
}

void Mesh::setShaders(const char **files, const GLenum *types, int nShaders) {
    std::vector<GLuint> shaders;
    for (int i = 0; i < nShaders; ++i) {
        char *source = loadTxtSource(files[i]);
        if (source == NULL) {
            Console::getInstance()->message(CONSOLE_MSG_SYS, "Failed to load shader %s", files[i]);
        } else {
            shaders.push_back(createShader(types[i], source));
            delete [] source;
        }
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
    
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
    
        static float lightX;
        if (lightX < 1000) {
            lightX += 1;
        } else {
            lightX = -1000;
        }
        
        
        if (m_shaderProgram > 0) {
            glUseProgramObjectARB(m_shaderProgram);
            GLuint lightPosition = glGetUniformLocationARB(m_shaderProgram, "lightPosition");
            GLuint lightDiffuse = glGetUniformLocationARB(m_shaderProgram, "lightDiffuse");
            GLuint materialDiffuse = glGetUniformLocationARB(m_shaderProgram, "materialDiffuse");
            GLuint materialAmbient = glGetUniformLocationARB(m_shaderProgram, "materialAmbient");
            GLuint lightAmbient = glGetUniformLocationARB(m_shaderProgram, "lightAmbient");
            GLuint globalAmbient = glGetUniformLocationARB(m_shaderProgram, "globalAmbient");
            glUniform3f(lightPosition, lightX, 400.0f, 0.0f);
            glUniform4f(lightDiffuse, 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(materialDiffuse, 1.0f, 0.0f, 0.0f, 1.0f);
            glUniform4f(materialAmbient, 0.0f, 0.0f, 0.0f, 0.0f);
            glUniform4f(lightAmbient, 0.0f, 0.0f, 0.0f, 0.0f);
            glUniform4f(globalAmbient, 0.3f, 0.0f, 0.0f, 1.0f);
        }
        
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
        
        if (m_attrPosition != -1) {
            glVertexAttribPointerARB(m_attrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
            glEnableVertexAttribArrayARB(m_attrPosition);
        }
        if (m_attrNormal != -1) {
            glVertexAttribPointerARB(m_attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)3);
            glEnableVertexAttribArrayARB(m_attrNormal);
        }
            
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
        
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_idVBOIndices);
        
        // Render
        //glEnable(GL_TEXTURE_2D);
        glDrawElements(GL_TRIANGLES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
        //glDrawArrays( GL_TRIANGLES, 0, m_nVertexCount );        // Draw All Of The Triangles At Once
        //glDisable(GL_TEXTURE_2D);
        
        glDisableVertexAttribArrayARB(m_attrPosition);
        glDisableVertexAttribArrayARB(m_attrNormal);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        
        if (m_shaderProgram > 0)
            glUseProgramObjectARB(0);
        
        for (int i = 0; i < m_nIndexCount; ++i) {
            Vert v = m_pVertices[m_pIndices[i]];
            glBegin(GL_LINES);
            glColor3d(1,1,1);
            glVertex3f(v.x, v.y, v.z);
            glVertex3f(v.x + v.nx * 10,
                       v.y + v.ny * 10,
                       v.z + v.nz * 10);
            glEnd();
        }
    }

}

void Mesh::buildVBOs() {
    
    // Generate And Bind The Vertex Buffer
    glGenBuffersARB( 1, &m_nVBOVertices );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
    glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(Vert) * m_nVertexCount, m_pVertices, m_memHint );
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0); // cleanup

    // Generate and Bind the index buffer
    glGenBuffersARB( 1, &m_idVBOIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idVBOIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nIndexCount * sizeof(short), m_pIndices, GL_STATIC_DRAW);
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER, 0); // cleanup
}


bool Mesh::loadHeightmap( const char* szPath, float flHeightScale, float flResolution ) {
    int sizeX, sizeY;
    
    // Load Texture Data
    m_pTextureImage = Texture::CreateFromFile(szPath);
    m_pTextureImage->size(&sizeX, &sizeY);
    
    // Generate Vertex Field
    int vertsInX = (int)(sizeX / flResolution) + 1;
    int vertsInZ = (int)(sizeY / flResolution) + 1;
    m_nVertexCount = vertsInX * vertsInZ;
    m_nIndexCount  = (vertsInX - 1) * (vertsInZ - 1) * 6;
    
    m_pVertices = new Vert[m_nVertexCount];         // Allocate Vertex Data
    m_pIndices  = new unsigned short[m_nIndexCount];
    m_pTexCoords = new TexCoord[m_nVertexCount];    // Allocate Tex Coord Data
    
    int nIndex=0;
    float flX, flZ;

    
    int nNormals[m_nVertexCount];
    for (int i = 0; i < m_nVertexCount; ++i) {
        nNormals[i] = 0;
    }
    
    //
    for( int z = 0; z < vertsInZ; ++z ) {
        for( int x = 0; x < vertsInX; ++x ) {
            flX = (float) x * flResolution;
            flZ = (float) z * flResolution;
            
            // Set The Data, Using PtHeight To Obtain The Y Value
            m_pVertices[nIndex].x = flX - ( sizeX / 2 );
            m_pVertices[nIndex].y = m_pTextureImage->height( (int) flX, (int) flZ ) *  flHeightScale;
            m_pVertices[nIndex].z = flZ - ( sizeY / 2 );
            
            // Stretch The Texture Across The Entire Mesh
            m_pTexCoords[nIndex].u = flX / sizeX;
            m_pTexCoords[nIndex].v = flZ / sizeY;
            
            // normals will be summed later
            m_pVertices[nIndex].nx = 0;
            m_pVertices[nIndex].ny = 1;
            m_pVertices[nIndex].nz = 0;
            
            // Increment Our Index
            nIndex++;
        }
    }
    
    int indicesIndex = 0;
    for( int z = 0; z < vertsInZ - 1; ++z ) {
        for( int x = 0; x < vertsInX - 1; ++x ) {
    
            unsigned short start = (unsigned short)(z * vertsInX + x);
            
            // calculate indices for two tris in a grid section
            m_pIndices[indicesIndex++] = start;
            m_pIndices[indicesIndex++] = (short)(start + vertsInX);
            m_pIndices[indicesIndex++] = (short)(start + 1);
            m_pIndices[indicesIndex++] = (short)(start + 1);
            m_pIndices[indicesIndex++] = (short)(start + vertsInX);
            m_pIndices[indicesIndex++] = (short)(start + 1 + vertsInX);
            
            // calculate vertex normals for each vertex involved in the 2-tri sections
            for (int i = 0; i < 2; ++i) {
                Vector3 vec1;
                Vector3 vec2;
                Vector3 norm;
                int offset = i * 3 + 1;
                Vert v0 = m_pVertices[m_pIndices[indicesIndex - offset]];
                Vert v1 = m_pVertices[m_pIndices[indicesIndex - offset - 1]];
                Vert v2 = m_pVertices[m_pIndices[indicesIndex - offset - 2]];
                vec1 = Vector3(v0.x - v1.x,
                               v0.y - v1.y,
                               v0.z - v1.z);
                vec2 = Vector3(v0.x - v2.x,
                               v0.y - v2.y,
                               v0.z - v2.z);
                norm = vec2.cross(vec1);
                norm.normalize();
                for (int j = 0; j < 3; ++j) {
                    m_pVertices[m_pIndices[indicesIndex - offset - j]].nx += norm.m_x;
                    m_pVertices[m_pIndices[indicesIndex - offset - j]].ny += norm.m_y;
                    m_pVertices[m_pIndices[indicesIndex - offset - j]].nz += norm.m_z;
                    nNormals[m_pIndices[indicesIndex - offset - j]] += 1;
                }
            }
        }
    }
    // finished the vertex normal calulation by averaging
    for (int i = 0; i < m_nVertexCount; ++i) {
        m_pVertices[i].nx /= nNormals[i];
        m_pVertices[i].ny /= nNormals[i];
        m_pVertices[i].nz /= nNormals[i];
    }
    
    // Load Vertex Data Into The Graphics Card Memory
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {
        buildVBOs();
    }
    
    return true;
}