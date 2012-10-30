//
//  GameObject.cpp
//  Triumph
//
//  Created by Justin Labak on 10/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "GameObject.h"

GameObject::GameObject()
{
	m_position = Vector3::zero;
	m_rotation = Quaternion::identity;
}

void GameObject::translate(Vector3 v)
{
    m_position += v;
}

void GameObject::rotate(const Vector3 &v, float angle)
{
    m_rotation *= Quaternion::CreateFromAxis(v, angle);
}