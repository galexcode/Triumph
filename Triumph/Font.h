//
//  Font.h
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.

#ifndef __Triumph__Font__
#define __Triumph__Font__

#include <GL/glfw.h>

class Font
{
public:
	Font(const char* filename);
	int init();
	void print(const char *text, int x, int y);
    void print(const char *text);
    void test();
	void clean();
private:
    GLuint m_fontOffset;
    static const GLubyte rasters[95][13];
    
    void makeRasterFont();
};

#endif /* defined(__Triumph__Font__) */