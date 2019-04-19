#pragma once

#include "Constants.h"



class Tuple
{
public:
    double x, y, z, w;

    Tuple();
    Tuple(double x, double y, double z, double w);
    ~Tuple() = default;

    //Creates a point (w = 1.0f)
    static Tuple CreatePoint(double x, double y, double z);
    //Creates a vector (w = 0.0f)
    static Tuple CreateVector(double x, double y, double z);
    //Calculating the dot product
    static double DotProduct(Tuple t1, Tuple t2);
    //Calculating the cross product
    static Tuple CrossProduct(Tuple t1, Tuple t2);
    //Calculate a vector's magnitude
    double Magnitude();
    //Normalize a vector (-> Magnitude = 1.0)
    Tuple Normalize();
    
    Tuple Reflect(Tuple normal) const;

    //Does the tuple represent a point?
    bool IsPoint();
    //Does the tuple represent a vector?
    bool IsVector();

    //Operator overloads
    bool operator== (const Tuple t) const;
    bool operator!= (const Tuple t) const;
    Tuple operator+ (const Tuple t) const;
    Tuple operator- (const Tuple t) const;
    Tuple operator- () const;
    Tuple operator* (const double t) const;
    Tuple operator/ (const double t) const;
};

using Point = Tuple;
using Vector = Tuple;

inline Tuple::Tuple()
{
   x = y = z = w = 0.0;
}

inline Tuple::Tuple(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

inline Tuple Tuple::CreatePoint(double x, double y, double z)
{
    return Tuple(x, y, z, 1.0);
}

inline Tuple Tuple::CreateVector(double x, double y, double z)
{
    return Tuple(x, y, z, 0.0);
}

inline double Tuple::DotProduct(Tuple t1, Tuple t2)
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

inline double Tuple::Magnitude()
{
    return sqrt(
        (x*x) +
        (y*y) +
        (z*z) +
        (w*w)
    );
}

inline Tuple Tuple::Normalize()
{
    double magnitude = Magnitude();

    return Tuple(
        x / magnitude,
        y / magnitude,
        z / magnitude,
        w / magnitude
    );
}

inline Tuple Tuple::Reflect(Tuple normal) const {
    return (*this) - (normal * 2.0 * DotProduct(*this, normal));
}

inline bool Tuple::IsPoint()
{
    return Constants::DoubleEqual(w, 1.0);
}

inline bool Tuple::IsVector()
{
    return Constants::DoubleEqual(w, 0.0);
}

inline bool Tuple::operator==(const Tuple t) const
{
    return
        Constants::DoubleEqual(x, t.x)
        && Constants::DoubleEqual(y, t.y)
        && Constants::DoubleEqual(z, t.z)
        && Constants::DoubleEqual(w, t.w);
}

inline bool Tuple::operator!=(const Tuple t) const
{
    return !(*this == t);
}

inline Tuple Tuple::operator+(const Tuple t) const
{
    return Tuple(x + t.x, y + t.y, z + t.z, w + t.w);
}

inline Tuple Tuple::operator-(const Tuple t) const
{
    return Tuple(x - t.x, y - t.y, z - t.z, w - t.w);
}

inline Tuple Tuple::operator-() const
{
    return Tuple(-x, -y, -z, -w);
}

inline Tuple Tuple::operator*(const double t) const
{
    return Tuple(x * t, y * t, z * t, w * t);
}

inline Tuple Tuple::operator/(const double t) const
{
    return Tuple(x / t, y / t, z / t, w / t);
}