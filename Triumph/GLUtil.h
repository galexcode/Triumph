//
//  GLUtil.h
//  Triumph
//
//  Created by Justin Labak on 11/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__GLUtil__
#define __Triumph__GLUtil__

#define BACK_ID 0
#define FRONT_ID 1
#define RIGHT_ID 2
#define LEFT_ID 3
#define TOP_ID 4
#define BOTTOM_ID 5

#include "Texture.h"
#include "Geometry.h"

class GLUtil {
public:
    // 3d utility functions
    static Ray3 GetMouseRay();
    
    // basic drawing functions
    static void DrawSkybox(Vector3 eye, Texture *textures[6]);
	static void DrawCube();
    
    // shader functions
    static GLuint CreateShader(GLenum eShaderType, const char *source);
    static GLuint CreateShaderProgram(GLuint *shaderList, int nShaders);
};

#endif /* defined(__Triumph__GLUtil__) */
