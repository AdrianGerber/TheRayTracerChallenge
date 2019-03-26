#pragma once

#include "Constants.h"

class Tuple
{
public:
    float x, y, z, w;

    Tuple();
    Tuple(float x, float y, float z, float w);
    ~Tuple() = default;

    //Creates a point (w = 1.0f)
    static Tuple CreatePoint(float x, float y, float z);
    //Creates a vector (w = 0.0f)
    static Tuple CreateVector(float x, float y, float z);
    //Calculating the dot product
    static float DotProduct(Tuple t1, Tuple t2);
    //Calculating the cross product
    static Tuple CrossProduct(Tuple t1, Tuple t2);
    //Calculate a vector's magnitude
    float Magnitude();
    //Normalize a vector (-> Magnitude = 1.0f)
    Tuple Normalize();

    //Does the tuple represent a point?
    bool IsPoint();
    //Does the tuple represent a vector?
    bool IsVector();

    //Operator overloads
    bool operator== (Tuple t);
    bool operator!= (Tuple t);
    Tuple operator+ (Tuple t);
    Tuple operator- (Tuple t);
    Tuple operator- ();
    Tuple operator* (float t);
    Tuple operator/ (float t);
};

Tuple::Tuple()
{
   x = y = z = w = 0.0f;
}

inline Tuple::Tuple(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

inline Tuple Tuple::CreatePoint(float x, float y, float z)
{
    return Tuple(x, y, z, 1.0f);
}

inline Tuple Tuple::CreateVector(float x, float y, float z)
{
    return Tuple(x, y, z, 0.0f);
}

inline float Tuple::DotProduct(Tuple t1, Tuple t2)
{
    return
        (t1.x * t2.x) +
        (t1.y * t2.y) +
        (t1.z * t2.z) +
        (t1.w * t2.w);
}

inline Tuple Tuple::CrossProduct(Tuple t1, Tuple t2)
{
    return Tuple::CreateVector(
        (t1.y * t2.z) - (t1.z * t2.y),
        (t1.z * t2.x) - (t1.x * t2.z),
        (t1.x * t2.y) - (t1.y * t2.x)
    );
}

inline float Tuple::Magnitude()
{
    return sqrtf(
        (x*x) +
        (y*y) +
        (z*z) +
        (w*w)
    );
}

inline Tuple Tuple::Normalize()
{
    float magnitude = Magnitude();

    return Tuple(
        x / magnitude,
        y / magnitude,
        z / magnitude,
        w / magnitude
    );
}

inline bool Tuple::IsPoint()
{
    return Constants::FloatEqual(w, 1.0f);
}

inline bool Tuple::IsVector()
{
    return Constants::FloatEqual(w, 0.0f);
}

inline bool Tuple::operator==(Tuple t)
{
    return
        Constants::FloatEqual(x, t.x)
        && Constants::FloatEqual(y, t.y)
        && Constants::FloatEqual(z, t.z)
        && Constants::FloatEqual(w, t.w);
}

inline bool Tuple::operator!=(Tuple t)
{
    return !(*this == t);
}

inline Tuple Tuple::operator+(Tuple t)
{
    return Tuple(x + t.x, y + t.y, z + t.z, w + t.w);
}

inline Tuple Tuple::operator-(Tuple t)
{
    return Tuple(x - t.x, y - t.y, z - t.z, w - t.w);
}

inline Tuple Tuple::operator-()
{
    return Tuple(-x, -y, -z, -w);
}

inline Tuple Tuple::operator*(float t)
{
    return Tuple(x * t, y * t, z * t, w * t);
}

inline Tuple Tuple::operator/(float t)
{
    return Tuple(x / t, y / t, z / t, w / t);
}