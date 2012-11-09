//
//  Game.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__Game__
#define __Triumph__Game__

class Game
{
    
public:
    virtual int init() { return 0; }
    
    virtual void update(float dTime) {}
    virtual void draw(float dTime) {}
    virtual void drawUI(float dTime) {}
    
    virtual void mouseMove(int x, int y) {}
    virtual void windowResize(int width, int height) {}
    virtual void keyEvent(int key, int state) {}
    virtual void mouseButtonEvent(int button, int state) {}
    virtual void mouseWheelEvent(int dir) {}
    
    virtual void clean() {}
    
};

#endif /* defined(__Triumph__Game__) */
