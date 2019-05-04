#pragma once
#include "Shape.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "IntersectionBuffer.h"
#include <cmath>
#include <limits>

namespace RayTracer {
	//A plane that extends infinitly in every direction
	class Plane : public Shape
	{
	public:
		Plane() = default;
		~Plane() = default;

		void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

		Vector FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) override;

		BoundingBox GetObjectSpaceBounds() override {
			constexpr auto inf = std::numeric_limits<double>::infinity();
			return BoundingBox(Point::CreatePoint(-inf, 0.0, -inf), Point::CreatePoint(inf, 0.0, inf));
		}

		void PartitionChildren(size_t maximumShapeCount) override {}

	private:
		std::shared_ptr<Shape> ShapeSpecificCopy() override {
			return Shape::MakeShared<Plane>(*this);
		}
	};

	inline void Plane::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer)
	{
		//Plane and ray parallel -> no intersections
		if (abs(ray.direction.y) < Constants::EPSILON) {
			return;
		}

		//How far does the ray need to travel, until it reaches the plane (at y = 0.0)
		double t = -ray.origin.y / ray.direction.y;

		buffer.Add(Intersection(t, GetPointer()));
	}

	inline Vector Plane::FindObjectSpaceNormal(Point p, const Intersection& globalIntersection)
	{
		//The normal vector does not depend on the point
		return Vector::CreateVector(0.0, 1.0, 0.0);
	}
}
