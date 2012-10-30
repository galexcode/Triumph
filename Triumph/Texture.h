//
//  Texture.h
//  Triumph
//
//  Created by Justin Labak on 10/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__Texture__
#define __Triumph__Texture__

#include <GL/glfw.h>

#include "Platform.h"

#if PLATFORM == PLATFORM_WIN32
#define FORMAT_BGR GL_BGR_EXT
#else
#define FORMAT_BGR GL_BGR
#endif

class Texture
{
public:
    static Texture * CreateFromFile(const char *file);
    GLuint gid();
    
    ~Texture();
    
private:
    bool m_fLoaded;
    GLuint m_gid;
	GLubyte *m_pImage;

	int m_width, m_height;
    
    void loadBMP(const char *filename);
};



#endif /* defined(__Triumph__Texture__) */
