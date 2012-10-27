//
//  Texture.cpp
//  Triumph
//
//  Created by Justin Labak on 10/26/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "Texture.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "Console.h"
#include "Bitmap.h"

Texture * Texture::CreateFromFile(const char *file)
{
    Texture *tex = new Texture();
	tex->m_fLoaded = false;
	tex->m_pImage = NULL;
    
    tex->loadBMP(file);
    
	if (tex->m_fLoaded)
	{

		glGenTextures(1, &tex->m_gid);
		glBindTexture(GL_TEXTURE_2D, tex->m_gid);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //NEAREST);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->m_width, tex->m_height, 0,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, tex->m_pImage);

	}
    
    return tex;
}

// currently assumes 24bit bitmaps
void Texture::loadBMP(const char *filename)
{   
    std::ifstream file(filename, std::ifstream::in | std::ifstream::binary);

    if(file.good()) { // file opened
        BITMAPFILEHEADER h;
		BITMAPINFOHEADER ih;
        
		file.read((char *)&h, sizeof(BITMAPFILEHEADER));
        file.read((char *)&ih, sizeof(BITMAPINFOHEADER));

		m_width = ih.biWidth;
		m_height = ih.biHeight;

		// account for potential row padding
		// or missing element count in ih
		int imgSize = (int)((m_width * ih.biBitCount + 31) / 32) * 4 * m_height;

		file.seekg(h.bfOffBits, std::ifstream::beg);

		// move to the start of the image data
		// delete the last image if it existed
		delete[] m_pImage;
		m_pImage = new GLubyte[imgSize];
		file.read((char *)m_pImage, imgSize);

		m_fLoaded = true;
    }

	file.close();
}


GLuint Texture::gid()
{
    return m_gid;
}

Texture::~Texture()
{
	delete[] m_pImage;
    glDeleteTextures(1, &m_gid);
}