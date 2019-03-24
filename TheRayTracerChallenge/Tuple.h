#pragma once
#include "Constants.h"

class Tuple
{
public:
	float x, y, z, w;

	Tuple();
	Tuple(float x, float y, float z, float w);
	~Tuple();

	static Tuple CreatePoint(float x, float y, float z);

	static Tuple CreateVector(float x, float y, float z);

	bool operator== (Tuple t);

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


Tuple::~Tuple()
{
}

inline Tuple Tuple::CreatePoint(float x, float y, float z)
{
	return Tuple(x, y, z, 1.0f);
}

inline Tuple Tuple::CreateVector(float x, float y, float z)
{
	return Tuple(x, y, z, 0.0f);
}

inline bool Tuple::operator==(Tuple t)
{
	return
		Constants::FloatEqual(x, t.x)
		&& Constants::FloatEqual(y, t.y)
		&& Constants::FloatEqual(z, t.z)
		&& Constants::FloatEqual(w, t.w);
}

inline bool Tuple::IsPoint()
{
	return Constants::FloatEqual(w, 1.0f);
}

inline bool Tuple::IsVector()
{
	return Constants::FloatEqual(w, 0.0f);
}
