//
//  Log.h
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__Console__
#define __Triumph__Console__

#include <iostream>
#include <cstdarg>

#include "Font.h"

#define LOG_FONTFILE "courer.bmp"
#define LOG_BUFFER_SIZE 6
#define LOG_MSG_LIMIT 256

#define CONSOLE_MSG_SYS 0
#define CONSOLE_MSG_USER 1

class Console
{
public:
    static Console* getInstance();
    
    void update(float dTime);
    void draw(float dTime);
    
    void initScreenDisplay();

	void message(int type, const char *fmt, ...);

    void addChar(char c);
    void delChar();
    void submit();
    
private:
	Font *m_font;
    bool m_fScreenDisplay;
    
    char *m_msgBuffer[LOG_BUFFER_SIZE];
    char *m_msg;
    int   m_msgLoc;
    
	Console();
    ~Console();

	void writeln(char *msg);

};


#endif /* defined(__Triumph__Console__) */
