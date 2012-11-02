//
//  GameObject.h
//  Triumph
//
//  Created by Justin Labak on 10/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef __Triumph__GameObject__
#define __Triumph__GameObject__

#include "Geometry.h"

class GameObject
{
    
public:
    Vector3 m_position;
    Quaternion m_rotation;
    
    float m_boundingRadius;
    
	GameObject();

    void translate(Vector3 v);
    void rotate(const Vector3 &v, float angle);
    
private:
    
    
    
};


#endif /* defined(__Triumph__GameObject__) */
