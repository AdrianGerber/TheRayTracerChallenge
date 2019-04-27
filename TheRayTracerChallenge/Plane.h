#pragma once
#include "Shape.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "IntersectionBuffer.h"
#include <cmath>

//A plane that extends infinitly in every direction
class Plane : public Shape
{
public:
	Plane() = default;
	~Plane() = default;

	IntersectionBuffer FindObjectSpaceIntersections(Ray ray) override;

	Vector FindObjectSpaceNormal(Point p) override;

private:
	std::shared_ptr<Shape> ShapeSpecificCopy() override{
		return Shape::MakeShared<Plane>(*this);
	}
};

inline IntersectionBuffer Plane::FindObjectSpaceIntersections(Ray ray)
{
	//Plane and ray parallel -> no intersections
	if (abs(ray.direction.y) < Constants::EPSILON) {
		return IntersectionBuffer();
	}

	//How far does the ray need to travel, until it reaches the plane (at y = 0.0)
	double t = -ray.origin.y / ray.direction.y;

	return IntersectionBuffer(Intersection(t, GetPointer()));
}

inline Vector Plane::FindObjectSpaceNormal(Point p)
{
	//The normal vector does not depend on the point
	return Vector::CreateVector(0.0, 1.0, 0.0);
}

