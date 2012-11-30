//
//  Geometry.cpp
//  Triumph
//
//  Created by Justin Labak on 10/29/12.
//  Copyright (c) 2012 Justin Labak. All rights reserved.
//

#include "Geometry.h"
#include <cmath>


Sphere::Sphere(const Vector3 &pos, const float r)
{
    m_pos = pos;
    m_radius = r;
}

Ray3::Ray3(const Vector3 &pos, const Vector3 &dir)
{
    m_pos = pos;
    m_dir = dir;
}

bool Ray3::intersects(const Sphere &s, Vector3 *loc)
{
    float a = m_dir.dot(m_dir);
    float b = m_dir.dot(2.0f * ( m_pos - s.m_pos));
    float c = s.m_pos.dot(s.m_pos) + m_pos.dot(m_pos)
        - 2.0f * m_pos.dot(s.m_pos) - s.m_radius*s.m_radius;
    float D = b*b + (-4.0f)*a*c;
    
    // If ray can not intersect then stop
    if (D < 0)
    {
        return false;
    }
    D=sqrtf(D);
    
    // Ray can intersect the sphere, solve the closer hitpoint
    float t = (-0.5f)*(b+D)/a;
    if (t > 0.0f)
    {
        Vector3 hit = m_pos + t * m_dir;
        loc->m_x = hit.m_x;
        loc->m_y = hit.m_y;
        loc->m_z = hit.m_z;
    }
    else
    {
        return false;
    }
    return true;
}

Vector3::Vector3()
{
    m_x = m_y = m_z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

Vector3 Vector3::zero(0, 0, 0);
Vector3 Vector3::up(0, 1, 0);
Vector3 Vector3::forward(0, 0, -1);
Vector3 Vector3::right(1, 0, 0);

Vector3 Vector3::operator+(const Vector3 &vec) const
{
    Vector3 v = *this;
    v += vec;
    return v;
}

Vector3 Vector3::operator-(const Vector3 &vec) const {
    Vector3 v = *this;
    v -= vec;
    return v;
}

Vector3 & Vector3::operator-=(const Vector3 &vec) {
    this->m_x -= vec.m_x;
    this->m_y -= vec.m_y;
    this->m_z -= vec.m_z;
    return *this;
}

Vector3 & Vector3::operator+=(const Vector3 &vec) {
    this->m_x += vec.m_x;
    this->m_y += vec.m_y;
    this->m_z += vec.m_z;
    return *this;
}

bool Vector3::operator==(const Vector3 &vec) const {
	return this->m_x == vec.m_x && this->m_y == vec.m_y && this->m_z == vec.m_z;
}

bool Vector3::operator!=(const Vector3 &vec) const {
	return !(*this == vec);
}

Vector3 operator*(const Vector3 &vec, const float f) {
    return Vector3(vec.m_x * f, vec.m_y * f, vec.m_z * f);
}

Vector3 operator*(const float f, const Vector3 &vec) {
    return vec * f;
}

bool Vector3::equals(const Vector3 &vec) const {
    return m_x == vec.m_x && m_y == vec.m_y && m_z == vec.m_z;
}

float Vector3::angleBetween(const Vector3 &vec) const {
    float sqrMag1 = sqrMagnitude();
    float sqrMag2 = vec.sqrMagnitude();
    if (sqrMag1 == 0 || sqrMag2 == 0)
        return 0;
    return acosf(dot(vec) / (sqrt(sqrMag1) * sqrt(sqrMag2)));
}

float Vector3::dot(const Vector3 &vec) const {
    return m_x * vec.m_x + m_y * vec.m_y + m_z * vec.m_z;
}

Vector3 Vector3::cross(const Vector3 &vec) const
{
    return Vector3(m_y * vec.m_z - m_z * vec.m_y,
                   m_z * vec.m_x - m_x * vec.m_z,
                   m_x * vec.m_y - m_y * vec.m_x);
}

void Vector3::normalize()
{
    float sqrMag = sqrMagnitude();
    if (fabs(sqrMag) > TOLERANCE && fabs(sqrMag - 1.0f) > TOLERANCE)
    {
        float mag = sqrt(sqrMag);
        m_x /= mag;
        m_y /= mag;
        m_z /= mag;
    }
}

float Vector3::sqrMagnitude() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

float Vector3::magnitude() const
{
    return sqrt(sqrMagnitude());
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
}

Quaternion::Quaternion()
{
    m_x = m_y = m_z = m_w = 0;
}

Quaternion Quaternion::identity(0, 0, 0, 1);

Quaternion Quaternion::operator*(const Quaternion &rhs)
{
	Quaternion q = *this;
	q *= rhs;
    return q;
}

Quaternion & Quaternion::operator*=(const Quaternion &rhs)
{
    float x = m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y;
    float y = m_w * rhs.m_y + m_y * rhs.m_w + m_z * rhs.m_x - m_x * rhs.m_z;
    float z = m_w * rhs.m_z + m_z * rhs.m_w + m_x * rhs.m_y - m_y * rhs.m_x;
    float w = m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z;
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
	return *this;
}

Quaternion Quaternion::conjugate()
{
    return Quaternion(-m_x, -m_y, -m_z, m_w);
}

float Quaternion::magnitude()
{
    return sqrt(sqrMagnitude());
}

float Quaternion::sqrMagnitude()
{
    return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

void Quaternion::normalize()
{
    float sqrMag = sqrMagnitude();
    if (fabs(sqrMag) > TOLERANCE && fabs(sqrMag - 1.0f) > TOLERANCE)
    {
        float mag = sqrt(sqrMag);
        m_w /= mag;
        m_x /= mag;
        m_y /= mag;
        m_z /= mag;
    }
}

Vector3 Quaternion::operator*(const Vector3 &vec)
{
    Vector3 vn(vec);
	// normalize vn
    float sqrMag = sqrMagnitude();
	float mag = sqrMag;
    if (fabs(sqrMag) > TOLERANCE && fabs(sqrMag - 1.0f) > TOLERANCE)
    {
        mag = sqrt(sqrMag);
        vn.m_x /= mag;
        vn.m_y /= mag;
        vn.m_z /= mag;
    }
    
    Quaternion vecQuat, resQuat;
    vecQuat.m_x = vn.m_x;
    vecQuat.m_y = vn.m_y;
    vecQuat.m_z = vn.m_z;
    vecQuat.m_w = 0;
    
    resQuat = vecQuat * conjugate();
    resQuat = *this * resQuat;
    
    return Vector3(resQuat.m_x, resQuat.m_y, resQuat.m_z) * mag;
}

Quaternion Quaternion::CreateFromAxis(const Vector3 &v, float angle)
{
	float sinAngle;
	angle *= 0.5f;
	Vector3 vn(v);
	vn.normalize();
    
	sinAngle = sin(angle);
    
	float x = (vn.m_x * sinAngle);
	float y = (vn.m_y * sinAngle);
	float z = (vn.m_z * sinAngle);
	float w = cos(angle);
    
    return Quaternion(x, y, z, w);
}

Quaternion Quaternion::CreateFromEuler(float pitch, float yaw, float roll)
{
	// Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	// and multiply those together.
	// the calculation below does the same, just shorter
    
	float p = pitch * PI / 180 / 2.0;
	float y = yaw * PI / 180 / 2.0;
	float r = roll * PI / 180 / 2.0;
    
	float sinp = sin(p);
	float siny = sin(y);
	float sinr = sin(r);
	float cosp = cos(p);
	float cosy = cos(y);
	float cosr = cos(r);
    
	float qx = sinr * cosp * cosy - cosr * sinp * siny;
	float qy = cosr * sinp * cosy + sinr * cosp * siny;
	float qz = cosr * cosp * siny - sinr * sinp * cosy;
	float qw = cosr * cosp * cosy + sinr * sinp * siny;
    
    Quaternion q(qx, qy, qz, qw);
    
	q.normalize();
    
    return q;
}

void Quaternion::getAxisAngle(Vector3 *axis, float *angle)
{
    float scale = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
    axis->m_x = m_x / scale;
    axis->m_y = m_y / scale;
    axis->m_z = m_z / scale;
    *angle = acos(m_w) * 2.0f;
}


Matrix4 Quaternion::getMatrix()
{
    // todo: get rid of this call
	normalize();

	float x2 = m_x * m_x;
	float y2 = m_y * m_y;
	float z2 = m_z * m_z;
	float xy = m_x * m_y;
	float xz = m_x * m_z;
	float yz = m_y * m_z;
	float wx = m_w * m_x;
	float wy = m_w * m_y;
	float wz = m_w * m_z;
    
    Matrix4 mat;
    
    mat.v[0] = 1.0f - 2.0f * (y2 + z2);
    mat.v[1] = 2.0f * (xy - wz);
    mat.v[2] = 2.0f * (xz + wy);
    mat.v[3] = 0.0f;
    
    mat.v[4] = 2.0f * (xy + wz);
    mat.v[5] = 1.0f - 2.0f * (x2 + z2);
    mat.v[6] = 2.0f * (yz - wx);
    mat.v[7] = 0.0f;
    
    mat.v[8] = 2.0f * (xz - wy);
    mat.v[9] = 2.0f * (yz + wx);
    mat.v[10] = 1.0f - 2.0f * (x2 + y2);
    mat.v[11] = 0.0f;
    
    mat.v[12] = 0.0f;
    mat.v[13] = 0.0f;
    mat.v[14] = 0.0f;
    mat.v[15] = 1.0f;
    
    return mat;
}
