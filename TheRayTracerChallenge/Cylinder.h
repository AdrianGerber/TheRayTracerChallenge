#pragma once
#include "Shape.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "IntersectionBuffer.h"
#include <cmath>
#include <limits>

//A Cylinder, either capped or not
class Cylinder : public Shape
{
public:
	Cylinder();
	Cylinder(double min, double max, bool isClosed);
	~Cylinder() = default;

	IntersectionBuffer FindObjectSpaceIntersections(Ray ray) override;

	Vector FindObjectSpaceNormal(Point p) override;

	//Limit the length of the cylinder
	void SetMaximum(double value) { maximum = value; }
	void SetMinimum(double value) { minimum = value; }
	double GetMaximum() { return maximum; }
	double GetMinimum() { return minimum; }

	//Is the cylinder closed on both ends?
	bool IsClosed() { return closed; }
	void SetClosed(bool isClosed) { closed = isClosed; }

private:
	double minimum, maximum;
	bool closed;

	//Check if the ray at 't' is inside the cylinder 
	bool CheckCap(Ray ray, double t);

	//Find the ray's intersections with the end caps
	IntersectionBuffer IntersectEndCaps(Ray ray);
};

inline Cylinder::Cylinder() {
	minimum = -std::numeric_limits<double>::infinity();
	maximum = std::numeric_limits<double>::infinity();
	closed = false;
}

inline Cylinder::Cylinder(double min, double max, bool isClosed) {
	minimum = min;
	maximum = max;
	closed = isClosed;
}

inline IntersectionBuffer Cylinder::FindObjectSpaceIntersections(Ray ray)
{
	double a = (ray.direction.x * ray.direction.x) + (ray.direction.z * ray.direction.z);

	//Ray is parallel to y axis -> ray can only hit the end caps
	if (Constants::DoubleEqual(a, 0.0)) {
		return IntersectEndCaps(ray);
	}

	double b = (2.0 * ray.origin.x * ray.direction.x)
		+ (2.0 * ray.origin.z * ray.direction.z);

	double c = (ray.origin.x * ray.origin.x) + (ray.origin.z * ray.origin.z) - 1.0;

	double discriminant = (b * b) - (4.0 * a * c);

	//No intersections with the walls -> ray can only hit the end caps
	if (discriminant < 0.0) {
		return IntersectEndCaps(ray);
	}

	double solution1 = (-b - sqrt(discriminant)) / (2.0 * a);
	double solution2 = (-b + sqrt(discriminant)) / (2.0 * a);

	if (solution1 > solution2) {
		std::swap(solution1, solution2);
	}

	IntersectionBuffer intersections;

	//Intersections with the walls
	double y1 = ray.origin.y + (solution1 * ray.direction.y);
	if (y1 < maximum && y1 > minimum) {
		intersections.Add(Intersection(solution1, GetID()));
	}
	double y2 = ray.origin.y + (solution2 * ray.direction.y);
	if (y2 < maximum && y2 > minimum) {
		intersections.Add(Intersection(solution2, GetID()));
	}
	//Intersections with the end caps
	intersections.Add(IntersectEndCaps(ray));

	return intersections;
}

inline Vector Cylinder::FindObjectSpaceNormal(Point p)
{
	double distanceSquared = (p.x * p.x) + (p.z * p.z);

	//Is the point on the end cap at 'maximum'
	if (distanceSquared < 1.0 && p.y >= maximum - Constants::EPSILON) {
		return Vector::CreateVector(0.0, 1.0, 0.0);
	}

	//Is the point on the end cap at 'minimum'
	if (distanceSquared < 1.0 && p.y <= minimum + Constants::EPSILON) {
		return Vector::CreateVector(0.0, -1.0, 0.0);
	}

	//Point is on the wall of the cylinder
	return Vector::CreateVector(p.x, 0.0, p.z);
}

inline bool Cylinder::CheckCap(Ray ray, double t) {
	double x = ray.origin.x + (t * ray.direction.x);
	double z = ray.origin.z + (t * ray.direction.z);

	return (x * x) + (z * z) <= 1.0;
}

inline IntersectionBuffer Cylinder::IntersectEndCaps(Ray ray) {
	//Make sure the caps exist and could possibly be hit by the ray
	if (!closed || Constants::DoubleEqual(ray.direction.y, 0.0)) {
		return IntersectionBuffer();
	}

	IntersectionBuffer intersections;

	//Check if the ray hit the cap at the 'minimum'
	double t = (minimum - ray.origin.y) / ray.direction.y;
	if (CheckCap(ray, t)) {
		intersections.Add(Intersection(t, GetID()));
	}

	//Check if the ray hit the cap at the 'maximum'
	t = (maximum - ray.origin.y) / ray.direction.y;
	if (CheckCap(ray, t)) {
		intersections.Add(Intersection(t, GetID()));
	}

	return intersections;
}