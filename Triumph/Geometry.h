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
    
    Vector3 operator+(const Vector3 &vec) const;
    Vector3 & operator+=(const Vector3 &vec);
    Vector3 operator-(const Vector3 &vec) const;
    Vector3 & operator-=(const Vector3 &vec);

	bool operator==(const Vector3 &vec) const;
	bool operator!=(const Vector3 &vec) const;
    
    friend Vector3 operator*(const Vector3 &vec, const float f);
    friend Vector3 operator*(const float f, const Vector3 &vec);
    
    float dot(const Vector3 &vec) const;
    Vector3 cross(const Vector3 &vec) const;
    
    void normalize();
    float sqrMagnitude() const;
    float magnitude() const;
    float angleBetween(const Vector3 &vec) const;
    bool equals(const Vector3 &vec) const;
};

class Sphere
{
public:
    Vector3 m_pos;
    float m_radius;
    
    Sphere(const Vector3 &pos, const float r);
};

class Ray3
{
public:
    Vector3 m_pos;
    Vector3 m_dir;
    
    Ray3(const Vector3 &pos, const Vector3 &dir);
    bool intersects(const Sphere &s, Vector3 *loc);
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
