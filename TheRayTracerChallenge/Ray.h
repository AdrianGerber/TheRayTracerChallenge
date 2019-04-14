#pragma once

#include "Tuple.h"
#include <cmath>
#include <memory>
#include "Transform.h"

class Ray
{
public:
	Point origin;
	Vector direction;

	//Constructors and destructors
	Ray();
	Ray(Point origin, Vector direction);
	~Ray();

	//Mathematical functions
	Point PositionAt (float t) const { return origin + (direction * t);}

    Ray Transform(Transform m);

};


//Constructors and destructors
inline Ray::Ray()
{
}

inline Ray::Ray(Point origin, Vector direction) {
    this->origin = origin;
    this->direction = direction;
}

inline Ray::~Ray()
{
}



//Mathematical functions

inline Ray Ray::Transform(::Transform m) {
    Ray r;
    r.direction = m * direction;
    r.origin = m * origin;
    return r;
}