#pragma once

#include "Tuple.h"
#include <limits>
#include "Ray.h"

namespace RayTracer {

	class BoundingBox {
	public:
		BoundingBox();
		BoundingBox(Point minPoint, Point maxPoint);
		~BoundingBox() = default;

		Point GetMax() const { return max; }
		Point GetMin() const { return min; }

		void SetMax(Point newMax) { max = newMax; }
		void SetMin(Point newMin) { min = newMin; }

		void Add(Point point);
		void Add(const BoundingBox box);

		bool Contains(const Point p);
		bool Contains(const BoundingBox box);

		BoundingBox ApplyTransform(Transform transform);

		bool CheckIntersection(Ray ray);

		std::pair<BoundingBox, BoundingBox> SplitBox();

	private:
		Point min, max;


		std::pair<double, double> CheckAxis(double origin, double direction, double min, double max);
	};

	inline BoundingBox::BoundingBox() {
		constexpr double infinity = std::numeric_limits<double>::infinity();
		min = Point::CreatePoint(infinity, infinity, infinity);
		max = Point::CreatePoint(-infinity, -infinity, -infinity);
	}

	inline BoundingBox::BoundingBox(Point minPoint, Point maxPoint) {
		max = maxPoint;
		min = minPoint;
	}

	inline void BoundingBox::Add(Point point) {
		if (min.x > point.x) min.x = point.x;
		if (min.y > point.y) min.y = point.y;
		if (min.z > point.z) min.z = point.z;

		if (max.x < point.x) max.x = point.x;
		if (max.y < point.y) max.y = point.y;
		if (max.z < point.z) max.z = point.z;
	}

	inline void BoundingBox::Add(const BoundingBox box) {
		Add(box.GetMin());
		Add(box.GetMax());
	}

	inline bool BoundingBox::Contains(const Point p) {
		return
			min.x <= p.x && max.x >= p.x &&
			min.y <= p.y && max.y >= p.y &&
			min.z <= p.z && max.z >= p.z
			;
	}

	inline bool BoundingBox::Contains(const BoundingBox box) {
		return
			Contains(box.GetMin())
			&& Contains(box.GetMax());
	}

	inline BoundingBox BoundingBox::ApplyTransform(Transform transform) {
		//All 8 corners of the bounding box
		std::array<Point, 8> corners = {
			min,
			Point::CreatePoint(min.x, min.y, max.z),
			Point::CreatePoint(min.x, max.y, min.z),
			Point::CreatePoint(min.x, max.y, max.z),
			Point::CreatePoint(max.x, min.y, min.z),
			Point::CreatePoint(max.x, min.y, max.z),
			Point::CreatePoint(max.x, max.y, min.z),
			max
		};

		BoundingBox result;

		for (auto& corner : corners) {
			//Apply the transformation to each corner
			Point transformedPoint = transform * corner;

			//Find a new bounding box that contains all corners
			result.Add(transformedPoint);
		}

		return result;
	}

	inline bool BoundingBox::CheckIntersection(Ray ray) {
		auto xValues = CheckAxis(ray.origin.x, ray.direction.x, min.x, max.x);
		auto yValues = CheckAxis(ray.origin.y, ray.direction.y, min.y, max.y);
		auto zValues = CheckAxis(ray.origin.z, ray.direction.z, min.z, max.z);

		double minimumT = fmax(xValues.first, fmax(yValues.first, zValues.first));
		double maximumT = fmin(xValues.second, fmin(yValues.second, zValues.second));

		if (minimumT > maximumT) {
			//There was no intersection with the bounding boy
			return false;
		}

		//Bounding box was hit by the ray
		return true;
	}



	inline std::pair<double, double> BoundingBox::CheckAxis(double origin, double direction, double min, double max) {
		//Division by 0.0 will result in +/- infinity, which is handled correctly by the comparisons
		double tMin = (min - origin) / direction;
		double tMax = (max - origin) / direction;

		//Always return the smaller t value as pair.first 
		if (tMin > tMax) {
			return std::pair<double, double>(tMax, tMin);
		}
		return std::pair<double, double>(tMin, tMax);
	}

	inline std::pair<BoundingBox, BoundingBox> BoundingBox::SplitBox() {
		//Lengths of the cube's sides
		double dx = max.x - min.x;
		double dy = max.y - min.y;
		double dz = max.z - min.z;


		Point middleMax = max;
		Point middleMin = min;


		//Split the cube's longest side in half (Determine the shared points)

		//dx is the longest side
		if (dx >= dy && dx >= dz) {
			middleMax.x = middleMin.x = min.x + (dx / 2.0);
		}
		//dy is the longest side
		else if (dy >= dz) {
			middleMax.y = middleMin.y = min.y + (dy / 2.0);
		}
		//dz is the longest side
		else {
			middleMax.z = middleMin.z = min.z + (dz / 2.0);
		}

		//Create two new boxes that share a side
		return std::make_pair<BoundingBox, BoundingBox>(
			BoundingBox(min, middleMax),
			BoundingBox(middleMin, max)
			);
	}

}