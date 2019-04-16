#include "Plane.h"



Plane::Plane()
{
}


Plane::~Plane()
{
}

IntersectionBuffer Plane::FindObjectSpaceIntersections(Ray ray)
{
	//Plane and ray parallel -> no intersections
	if (abs(ray.direction.y) < Constants::EPSILON) {
		return IntersectionBuffer();
	}

	//How far does the ray need to travel, until it reaches the plane (at y = 0.0)
	double t = -ray.origin.y / ray.direction.y;

	return IntersectionBuffer(Intersection(t, GetID()));
}

Vector Plane::FindObjectSpaceNormal(Point p)
{
	return Vector::CreateVector(0.0, 1.0, 0.0);
}
