#pragma once
#include "Constants.h"

class Tuple
{
public:
	float x, y, z, w;

	Tuple();
	Tuple(float x, float y, float z, float w);
	~Tuple() = default;

	static Tuple CreatePoint(float x, float y, float z);

	static Tuple CreateVector(float x, float y, float z);

	static float DotProduct(Tuple t1, Tuple t2);

	static Tuple CrossProduct(Tuple t1, Tuple t2);

	bool operator== (Tuple t);

	Tuple operator+ (Tuple t);

	Tuple operator- (Tuple t);

	Tuple operator- ();

	Tuple operator* (float t);

	Tuple operator/ (float t);

	float Magnitude();

	Tuple Normalize();

	bool IsPoint();

	bool IsVector();
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

inline bool Tuple::operator==(Tuple t)
{
	return
		Constants::FloatEqual(x, t.x)
		&& Constants::FloatEqual(y, t.y)
		&& Constants::FloatEqual(z, t.z)
		&& Constants::FloatEqual(w, t.w);
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
