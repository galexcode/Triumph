//
//  GameCamera.h
//  Triumph
//
//  Created by Justin Labak on 10/31/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__GameCamera__
#define __Triumph__GameCamera__

#include "Geometry.h"

class GameCamera
{
public:
    Vector3 m_eye;
	Vector3 m_target;
	Vector3 m_up;

	GameCamera();
	GameCamera(Vector3 eye, Vector3 target, Vector3 up);
};

#endif /* defined(__Triumph__GameCamera__) */
