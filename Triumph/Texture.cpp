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
#include <math.h>

#include "Bitmap.h"

Texture * Texture::CreateGaussian(int cosAngleResolution, float specularShine)
{
    Texture *tex = new Texture();
	tex->m_fLoaded = true;
	tex->m_pImage = NULL;
    
    tex->m_pImage = BuildGaussianData(cosAngleResolution, specularShine);
    tex->m_width = cosAngleResolution;
    tex->m_height = 1;
    
    glGenTextures(1, &tex->m_gid);
    glBindTexture(GL_TEXTURE_1D, tex->m_gid);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, cosAngleResolution, 0,
                 GL_RED, GL_UNSIGNED_BYTE, tex->m_pImage);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
    glBindTexture(GL_TEXTURE_1D, 0); // cleanup
    
    return tex;
}

GLubyte * Texture::BuildGaussianData(int cosAngleResolution, float specularShine)
{
    GLubyte *data = new GLubyte[cosAngleResolution];
    
    for(int iCosAng = 0; iCosAng < cosAngleResolution; ++iCosAng)
    {
        float cosAng = iCosAng / (float)(cosAngleResolution - 1);
        float angle = acosf(cosAng);
        float exponent = angle / specularShine;
        exponent = -(exponent * exponent);
        float gaussianTerm = exp(exponent);
        
        data[iCosAng] = (GLubyte)(gaussianTerm * 255.0f);
    }
    
    return data;
}

Texture * Texture::CreateFromFile(const char *file)
{
    Texture *tex = new Texture();
	tex->m_fLoaded = false;
	tex->m_pImage = NULL;
    
    tex->loadBMP(file);
    
	if (tex->m_fLoaded)
	{
        
		glGenTextures(1, &tex->m_gid);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, tex->m_gid);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->m_width, tex->m_height, 0, FORMAT_BGR, GL_UNSIGNED_BYTE, tex->m_pImage);
        //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, tex->m_width, tex->m_height, FORMAT_BGR, GL_UNSIGNED_BYTE, tex->m_pImage);
        
        glBindTexture(GL_TEXTURE_2D, 0); // cleanup

	}
    
    return tex;
}

float Texture::height( int x, int y ) {
    // Calculate The Position In The Texture, Careful Not To Overflow
    int nPos = ( ( x % m_width )  + ( ( y % m_height ) * m_width ) ) * 3;
    
    float flR = (float) m_pImage[ nPos ];        
    float flG = (float) m_pImage[ nPos + 1 ];
    float flB = (float) m_pImage[ nPos + 2 ];
    
    // Calculate The Height Using The Luminance Algorithm
    return ( 0.299f * flR + 0.587f * flG + 0.114f * flB );
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


GLuint Texture::gid() const {
    return m_gid;
}

void Texture::size(int *w, int *h) const {
    *w = m_width;
    *h = m_height;
}

GLubyte * Texture::image() const {
    return m_pImage;
}

Texture::~Texture()
{
	delete[] m_pImage;
    glDeleteTextures(1, &m_gid);
}
