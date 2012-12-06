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
#include <sstream>
#include <string>
#include "GLUtil.h"

// GPU has VBO at this point
Mesh::Mesh() {
    
    m_memHint = GL_STATIC_DRAW_ARB;
    m_vertMod = NULL;
    
    m_shaderProgram = 0;

	m_pWVertices = NULL;
    
    m_vertMod = new VertMod(FUNC_GERSTNER);
}

void Mesh::setMemHint(int hint) {
    m_memHint = hint;
}

Mesh::~Mesh() {
    delete [] m_pVertices; m_pVertices = NULL;
    delete [] m_pTexCoords; m_pTexCoords = NULL;
    
	delete[] m_pWVertices; m_pWVertices = NULL;

    delete[] m_pIndices; m_pIndices = NULL;
    
    delete m_vertMod;
    
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
    
    m_attrPosition = glGetAttribLocationARB(*program, "AttrPosition");
    m_attrNormal   = glGetAttribLocationARB(*program, "AttrNormal");
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

void Mesh::draw(float dTime) {
    
    if (GameEngine::getInstance()->m_fGLSupportedVBO) {

        static Vector3 light = Vector3(-1000, 400, 0);
        if (light.m_x < 1000) {
            light.m_x += 1;
        } else {
            light.m_x = -1000;
        }
        
        glPushMatrix();
        glTranslated(light.m_x, light.m_y, light.m_z);
        glScaled(10, 10, 10);
        GLUtil::DrawCube();
        glPopMatrix();
        
        if (m_shaderProgram > 0) {
            glUseProgramObjectARB(m_shaderProgram);
            GLuint LightPosition = glGetUniformLocationARB(m_shaderProgram, "LightPosition");
            GLuint DiffuseColor = glGetUniformLocationARB(m_shaderProgram, "DiffuseColor");
            GLuint AmbientColor = glGetUniformLocationARB(m_shaderProgram, "AmbientColor");
            GLuint SpecularColor = glGetUniformLocationARB(m_shaderProgram, "SpecularColor");
            GLuint DiffuseIntensity = glGetUniformLocationARB(m_shaderProgram, "DiffuseIntensity");
            GLuint AmbientIntensity = glGetUniformLocationARB(m_shaderProgram, "AmbientIntensity");
            GLuint SpecularIntensity = glGetUniformLocationARB(m_shaderProgram, "SpecularIntensity");
            GLuint Roughness = glGetUniformLocationARB(m_shaderProgram, "Roughness");
            GLuint Time = glGetUniformLocationARB(m_shaderProgram, "Time");
            
            glUniform1fARB(Time, GameEngine::getInstance()->getElapsedTime());
            
            glUniform3fARB(LightPosition, light.m_x, light.m_y, light.m_z);
            glUniform3fARB(DiffuseColor, 1.0f, 0.0f, 0.0f);
            glUniform3fARB(AmbientColor, 1.0f, 0.0f, 0.0f);
            glUniform3fARB(SpecularColor, 1.0f, 1.0f, 1.0f);
            glUniform1fARB(DiffuseIntensity, 0.2f);
            glUniform1fARB(AmbientIntensity, 0.2f);
            glUniform1fARB(SpecularIntensity, 0.2f);
            glUniform1fARB(Roughness, 0.5f);
            
            glUniform1iARB(glGetUniformLocationARB(m_shaderProgram, "NumWaves"), m_vertMod->nWaves);
            
            m_vertMod->update(dTime);
            
            for (int i = 0; i < m_vertMod->nWaves; ++i) {
                std::stringstream uniform;
                GLuint w;
                
                uniform << "Waves[" << i << "].amp";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform1fARB(w, m_vertMod->waves[i].amp);
                uniform.str(std::string());
                
                uniform << "Waves[" << i << "].freq";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform1fARB(w, m_vertMod->waves[i].freq);
                uniform.str(std::string());
                
                uniform << "Waves[" << i << "].phase";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform1fARB(w, m_vertMod->waves[i].phase);
                uniform.str(std::string());
                
                uniform << "Waves[" << i << "].exp";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform1fARB(w, m_vertMod->waves[i].exp);
                uniform.str(std::string());
                
                uniform << "Waves[" << i << "].steepness";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform1fARB(w, m_vertMod->waves[i].steepness);
                uniform.str(std::string());
                
                uniform << "Waves[" << i << "].dir";
                w = glGetUniformLocationARB(m_shaderProgram, uniform.str().c_str());
                glUniform2fARB(w, m_vertMod->waves[i].dir.m_x, m_vertMod->waves[i].dir.m_z);
            }
        }
        
        glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
        
        if (m_attrPosition != -1) {
            glVertexAttribPointerARB(m_attrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
            glEnableVertexAttribArrayARB(m_attrPosition);
        }
        if (m_attrNormal != -1) {
            glVertexAttribPointerARB(m_attrNormal, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, ((char*)NULL)+3*sizeof(float));
            glEnableVertexAttribArrayARB(m_attrNormal);
        }
        
        glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_idVBOIndices);
        
        // Render
        glDrawElements(GL_TRIANGLES, m_nIndexCount, GL_UNSIGNED_SHORT, 0);
        
        // Cleanup
        glDisableVertexAttribArrayARB(m_attrPosition);
        glDisableVertexAttribArrayARB(m_attrNormal);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
        
        if (m_shaderProgram > 0)
            glUseProgramObjectARB(0);
        
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