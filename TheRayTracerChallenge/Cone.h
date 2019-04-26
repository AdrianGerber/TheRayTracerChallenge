#pragma once
#include "Shape.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "IntersectionBuffer.h"
#include <cmath>
#include <limits>

//A Cone, either capped or not
class Cone : public Shape
{
public:
	Cone();
	Cone(double min, double max, bool isClosed);
	~Cone() = default;

	IntersectionBuffer FindObjectSpaceIntersections(Ray ray) override;

	Vector FindObjectSpaceNormal(Point p) override;

	//Limit the length of the cone
	void SetMaximum(double value) { maximum = value; }
	void SetMinimum(double value) { minimum = value; }
	double GetMaximum() { return maximum; }
	double GetMinimum() { return minimum; }

	//Is the cone closed on both ends?
	bool IsClosed() { return closed; }
	void SetClosed(bool isClosed) { closed = isClosed; }

private:
	double minimum, maximum;
	bool closed;

	//Check if the ray at 't' is inside the cone 
	bool CheckCap(Ray ray, double t, double radius);

	//Find the ray's intersections with the end caps
	IntersectionBuffer IntersectEndCaps(Ray ray);
};

inline Cone::Cone() {
	minimum = -std::numeric_limits<double>::infinity();
	maximum = std::numeric_limits<double>::infinity();
	closed = false;
}

inline Cone::Cone(double min, double max, bool isClosed) {
	minimum = min;
	maximum = max;
	closed = isClosed;
}

inline IntersectionBuffer Cone::FindObjectSpaceIntersections(Ray ray)
{
	IntersectionBuffer intersections;


	double a = (ray.direction.x * ray.direction.x) - (ray.direction.y * ray.direction.y) + (ray.direction.z * ray.direction.z);

	double b = (2.0 * ray.origin.x * ray.direction.x)
		- (2.0 * ray.origin.y * ray.direction.y)
		+ (2.0 * ray.origin.z * ray.direction.z);

	double c = (ray.origin.x * ray.origin.x) - (ray.origin.y * ray.origin.y) + (ray.origin.z * ray.origin.z);

	double discriminant = (b * b) - (4.0 * a * c);

	//Ray is parallel to a wall -> only 1 intersection possible
	if (Constants::DoubleEqual(a, 0.0)) {
		if (!Constants::DoubleEqual(b, 0.0)) {
			double t = -c / (2.0 * b);
			double y0 = ray.origin.y + (t * ray.direction.y);

			//Make sure the hit is inside the cone's limits
			if (y0 < maximum && y0 > minimum) {
				intersections.Add(Intersection(t, GetPointer()));
			}
		}		
	}

	//Multiple intersections possible
	else if (discriminant >= 0.0) {
		
		double solution1 = (-b - sqrt(discriminant)) / (2.0 * a);
		double solution2 = (-b + sqrt(discriminant)) / (2.0 * a);

		if (solution1 > solution2) {
			std::swap(solution1, solution2);
		}

		//Intersections with the walls
		double y1 = ray.origin.y + (solution1 * ray.direction.y);
		if (y1 < maximum && y1 > minimum) {
			intersections.Add(Intersection(solution1, GetPointer()));
		}
		double y2 = ray.origin.y + (solution2 * ray.direction.y);
		if (y2 < maximum && y2 > minimum) {
			intersections.Add(Intersection(solution2, GetPointer()));
		}
	}
	
	//Intersections with the end caps
	intersections.Add(IntersectEndCaps(ray));
	return intersections;
}

inline Vector Cone::FindObjectSpaceNormal(Point p)
{
	double distance = sqrt((p.x * p.x) + (p.z * p.z));

	if (closed) {
		//Is the point on the end cap at 'maximum'
		if (p.y >= maximum - Constants::EPSILON) {
			return Vector::CreateVector(0.0, 1.0, 0.0);
		}

		//Is the point on the end cap at 'minimum'
		if (p.y <= minimum + Constants::EPSILON) {
			return Vector::CreateVector(0.0, -1.0, 0.0);
		}
	}

	//Point is on the wall of the cone
	if (p.y > 0.0) {
		return Vector::CreateVector(p.x, -distance, p.z);
	}

	return Vector::CreateVector(p.x, distance, p.z);
}

inline bool Cone::CheckCap(Ray ray, double t, double radius) {
	
	double x = ray.origin.x + (t * ray.direction.x);
	double z = ray.origin.z + (t * ray.direction.z);

	return (x * x) + (z * z) <= radius * radius;
	
	return false;
}

inline IntersectionBuffer Cone::IntersectEndCaps(Ray ray) {
	
	//Make sure the caps exist
	if (!closed) {
		return IntersectionBuffer();
	}

	IntersectionBuffer intersections;

	//Check if the ray hit the cap at the 'minimum'
	double t = (minimum - ray.origin.y) / ray.direction.y;
	if (CheckCap(ray, t, minimum)) {
		intersections.Add(Intersection(t, GetPointer()));
	}

	//Check if the ray hit the cap at the 'maximum'
	t = (maximum - ray.origin.y) / ray.direction.y;
	if (CheckCap(ray, t, maximum)) {
		intersections.Add(Intersection(t, GetPointer()));
	}

	return intersections;
	
}