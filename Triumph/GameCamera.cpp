//
//  GameCamera.cpp
//  Triumph
//
//  Created by Justin Labak on 10/31/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GameCamera.h"

GameCamera::GameCamera()
{
	m_eye = Vector3::zero;
	m_target = Vector3::zero;
	m_up = Vector3::up;
}

GameCamera::GameCamera(Vector3 eye, Vector3 target, Vector3 up)
{
	m_eye = eye;
	m_target = target;
	m_up = up;
}