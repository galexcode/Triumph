//
//  Log.cpp
//  Triumph
//
//  Created by Justin Labak on 10/24/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "Console.h"
#include "GameEngine.h"
#include <string.h>

Console* Console::getInstance()
{
    static Console console;
    return &console;
}

Console::Console() 
{
	m_fScreenDisplay = false;
    
    // clear the message
    m_msg = new char[LOG_MSG_LIMIT];
    m_msg[0] = '\0';
    m_msgLoc = 0;
    
    // clear the console
    for (int i = 0; i < LOG_BUFFER_SIZE; ++i)
    {
        m_msgBuffer[i] = new char[LOG_MSG_LIMIT];
        m_msgBuffer[i][0] = '\0';
    }
    
}
Console::~Console()
{
    delete[] m_msg;
}

void Console::initScreenDisplay()
{
    m_font = new Font(LOG_FONTFILE);
    m_font->init();
	m_fScreenDisplay = true;
}

void Console::message(int type, const char *fmt, ...)
{
	char tmpBuf[LOG_MSG_LIMIT];

	// this memory will be freed in the writeln function when it exits the console buffer
	char *buf = new char[LOG_MSG_LIMIT];
    va_list args;
    va_start(args, fmt);
    vsprintf(&(tmpBuf[0]), fmt, args);
    va_end(args);

	sprintf(buf, "Sys: %s", &(tmpBuf[0]));

	writeln(buf);
}

void Console::writeln(char *msg)
{   
    // clear the string memory for the oldest msg
    delete[] m_msgBuffer[0];
    
    // move the strings toward the end of the buffer
    for (int i = 1; i < LOG_BUFFER_SIZE; ++i)
    {
        m_msgBuffer[i - 1] = m_msgBuffer[i];
    }
    
    // add the new string memory
    m_msgBuffer[LOG_BUFFER_SIZE - 1] = msg;
    
	printf("%s\n", msg);
    
}

void Console::addChar(char c)
{
    if (m_msgLoc <= LOG_MSG_LIMIT)
    {
        m_msg[m_msgLoc++] = c;
    }
}

void Console::delChar()
{
    if (m_msgLoc > 0)
    {
        m_msg[m_msgLoc++] = '\0';
    }
}

void Console::submit()
{
    writeln(m_msg);
    m_msg[0] = '\0';
}

void Console::update(float dTime)
{

}

void Console::draw(float dTime)
{
    if (m_fScreenDisplay)
    {
        int w, h;
        GameEngine::getInstance()->getWindowSize(&w, &h);
        
        for (int i = 0; i < LOG_BUFFER_SIZE; ++i)
        {
            m_font->print(m_msgBuffer[i], 10, h - 14 - 20 * i);
        }
        m_font->print(m_msg, 10, h - 14 - 20 * (LOG_BUFFER_SIZE));
    }
}