//
//  Geometry.h
//  Triumph
//
//  Created by Justin Labak on 10/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#ifndef Triumph_Geometry_h
#define Triumph_Geometry_h

#define TOLERANCE 0.00001f
#define PI        3.14f

typedef struct
{
    float v[16];
} Matrix4;

class Vector3
{
public:
 
    float m_x, m_y, m_z;
    
    static Vector3 zero;
    static Vector3 up;
    static Vector3 forward;
    static Vector3 right;
    
    Vector3();
    Vector3(float x, float y, float z);
    
    Vector3 operator+(const Vector3 &vec);
    Vector3 & operator+=(const Vector3 &vec);
    
    void normalize();
    float sqrMagnitude();
    float magnitude();
};

class Quaternion
{
  
public:
    float m_x, m_y, m_z, m_w;
    
	static Quaternion identity;

    Quaternion();
    Quaternion(float x, float y, float z, float w);
    
    static Quaternion CreateFromAxis(const Vector3 &v, float angle);
    static Quaternion CreateFromEuler(float pitch, float yaw, float roll);
    
    Quaternion operator*(const Quaternion &rhs);
	Quaternion & operator*=(const Quaternion &rhs);
    Vector3    operator*(const Vector3 &vec);
    
    Matrix4 getMatrix();
    float sqrMagnitude();    
    float magnitude();
    void normalize();
    void getAxisAngle(Vector3 *axis, float *angle);
    
private:
    Quaternion conjugate();
    
};

#endif
