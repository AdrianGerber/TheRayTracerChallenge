#pragma once

#include "Transform.h"
#include "Tuple.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Shape.h"

namespace RayTracer {
	//A sphere of radius 1 (unless transformed)
	class Sphere : public Shape
	{
	public:
		Sphere() = default;
		~Sphere() = default;

		//Virtual methods that need to be implemented
		void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

		Vector FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) override;

		BoundingBox GetObjectSpaceBounds() override {
			return BoundingBox(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));
		}

		void PartitionChildren(size_t maximumShapeCount) override {}

	private:
		std::shared_ptr<Shape> ShapeSpecificCopy() override {
			return Shape::MakeShared<Sphere>(*this);
		}
	};

	inline void Sphere::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) {
		//Vector from the sphere's center to the ray's origin (Sphere centered at 0/0/0)
		Vector sphereToRayOrigin = ray.origin - Point::CreatePoint(0.0, 0.0, 0.0);

		//Calculating the discriminant
		double a = Vector::DotProduct(ray.direction, ray.direction);
		double b = 2.0 * Vector::DotProduct(ray.direction, sphereToRayOrigin);
		double c = Vector::DotProduct(sphereToRayOrigin, sphereToRayOrigin) - 1.0;

		double discriminant = (b * b) - (4.0 * a * c);


		//No intersections
		if (discriminant < 0) {
			return;
		}

		Intersection i1, i2;
		i1.t = (-b - sqrt(discriminant)) / (2.0 * a);
		i2.t = (-b + sqrt(discriminant)) / (2.0 * a);

		i1.shape = GetPointer();
		i2.shape = GetPointer();

		buffer.Add(i1);
		buffer.Add(i2);
	}

	inline Vector Sphere::FindObjectSpaceNormal(Point p, const Intersection & globalIntersection) {
		return p - Point::CreatePoint(0.0, 0.0, 0.0);
	}
}
