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
#define GL_CLAMP_TO_EDGE 0x812F // windows does not support gl 1.2+
#define GL_GENERATE_MIPMAP 0x8191 // windows does not support gl 1.2+
#else
#define FORMAT_BGR GL_BGR
#endif

class Texture
{
public:
    static Texture * CreateFromFile(const char *file);
    GLuint gid() const;
    void size(int *w, int *h) const;
    GLubyte * image() const;
    
    ~Texture();
    
private:
    bool m_fLoaded;
    GLuint m_gid;
	GLubyte *m_pImage;

	int m_width, m_height;
    
    void loadBMP(const char *filename);
};



#endif /* defined(__Triumph__Texture__) */
