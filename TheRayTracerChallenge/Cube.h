#pragma once
#include "Shape.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "IntersectionBuffer.h"
#include <cmath>

namespace RayTracer {
	//Axis aligned bounding box (AABB)
	class Cube : public Shape
	{
	public:
		Cube() = default;
		~Cube() = default;

		void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

		Vector FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) override;

		BoundingBox GetObjectSpaceBounds() override {
			return BoundingBox(Point::CreatePoint(-1.0, -1.0, -1.0), Point::CreatePoint(1.0, 1.0, 1.0));
		}

		void PartitionChildren(size_t maximumShapeCount) override {}

	private:

		std::shared_ptr<Shape> ShapeSpecificCopy() override {
			return Shape::MakeShared<Cube>(*this);
		}

		//Calculate the ray's 't value' at the points '-1.0' and '0.0'
		std::pair<double, double> checkAxis(double origin, double direction);
	};

	inline void Cube::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer)
	{
		//Calculating the ray's 't' value for every plane where it could intersect the cube (each of the 6 sides)
		auto xValues = checkAxis(ray.origin.x, ray.direction.x);
		auto yValues = checkAxis(ray.origin.y, ray.direction.y);
		auto zValues = checkAxis(ray.origin.z, ray.direction.z);

		double minimumT = fmax(xValues.first, fmax(yValues.first, zValues.first));
		double maximumT = fmin(xValues.second, fmin(yValues.second, zValues.second));

		if (minimumT > maximumT) {
			//There was no intersection with the cube
			return;
		}
		buffer.Add(Intersection(minimumT, GetPointer()));
		buffer.Add(Intersection(maximumT, GetPointer()));
	}

	inline Vector Cube::FindObjectSpaceNormal(Point p, const Intersection& globalIntersection)
	{
		//Determine the axis that the surface normal should be alligned with
		//The largest coordinate value of the point corresponds to this axis
		double absX = fabs(p.x);
		double absY = fabs(p.y);
		double absZ = fabs(p.z);
		double maxCoordinate = fmax(absX, fmax(absY, absZ));

		//X direction
		if (Constants::DoubleEqual(maxCoordinate, absX)) {
			return Vector::CreateVector(p.x, 0.0, 0.0);
		}

		//Y direction
		if (Constants::DoubleEqual(maxCoordinate, absY)) {
			return Vector::CreateVector(0.0, p.y, 0.0);
		}

		//Z direction
		return Vector::CreateVector(0.0, 0.0, p.z);
	}


	inline std::pair<double, double> Cube::checkAxis(double origin, double direction) {
		//Division by 0.0 will result in +/- infinity, which is handled correctly by the comparisons
		double tMin = (-1.0 - origin) / direction;
		double tMax = (1.0 - origin) / direction;

		//Always return the smaller t value as pair.first 
		if (tMin > tMax) {
			return std::pair<double, double>(tMax, tMin);
		}
		return std::pair<double, double>(tMin, tMax);
	}

}