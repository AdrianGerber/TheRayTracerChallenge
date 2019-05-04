#pragma once

#include "Tuple.h"
#include <cmath>
#include <memory>
#include "Transform.h"

namespace RayTracer {
	//A 'Ray' that is used to trace the path that light would take (In reverse direction)
	class Ray
	{
	public:
		Point origin;
		Vector direction;

		//Constructors and destructors
		Ray();
		Ray(Point origin, Vector direction);
		~Ray();

		//Determine the ray's position after a certain distance is traveld
		Point PositionAt(double t) const { return origin + (direction * t); }

		//Create a new ray by applying transformations to this one
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

	inline Ray Ray::Transform(RayTracer::Transform m) {
		Ray r;
		r.direction = m * direction;
		r.origin = m * origin;
		return r;
	}
}
