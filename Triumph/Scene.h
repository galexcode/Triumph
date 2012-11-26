//
//  Scene.h
//  Triumph
//
//  Created by Justin Labak on 11/8/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__Scene__
#define __Triumph__Scene__

#include "Game.h"

class Scene : public Game
{
    
public:
	virtual int load() { return 0; }
    
};

#endif /* defined(__Triumph__Scene__) */
