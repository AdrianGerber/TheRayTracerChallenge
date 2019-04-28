#pragma once

#include "Shape.h"

class Triangle : public Shape {
public:

	Triangle() = default;
	Triangle(Point point0, Point point1, Point point2);
	~Triangle() = default;

	void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

	Vector FindObjectSpaceNormal(Point p) override { return normal; }

	void PartitionChildren(size_t maximumShapeCount) override {}

	BoundingBox GetObjectSpaceBounds() override;

	Point GetPoint(size_t nr);
	Vector GetEdge(size_t nr);

private:
	Point p0, p1, p2;
	Vector e0, e1, normal;

	void CalculateAttributes();

	std::shared_ptr<Shape> ShapeSpecificCopy() override {
		return Shape::MakeShared<Triangle>(*this);
	}
};

inline Triangle::Triangle(Point point0, Point point1, Point point2) {
	p0 = point0;
	p1 = point1;
	p2 = point2;

	CalculateAttributes();
}

inline Point Triangle::GetPoint(size_t nr) {
	switch (nr) {
	case 0: return p0;
	case 1: return p1;
	case 2: return p2;
	default: return p0;
	}
}

inline Vector Triangle::GetEdge(size_t nr) {
	switch (nr) {
	case 0: return e0;
	case 1: return e1;
	default: return e0;
	}
}

inline void Triangle::CalculateAttributes() {
	e0 = p1 - p0;
	e1 = p2 - p0;

	normal = Vector::CrossProduct(e1, e0).Normalize();
}

inline BoundingBox Triangle::GetObjectSpaceBounds() {
	BoundingBox bounds;

	bounds.Add(p0);
	bounds.Add(p1);
	bounds.Add(p2);

	return bounds;
}

inline void Triangle::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) {
	auto dirCrossE1 = Vector::CrossProduct(ray.direction, e1);
	double det = Vector::DotProduct(e0, dirCrossE1);
	//No intersection
	if (fabs(det) < Constants::EPSILON) return;

	double f = 1.0 / det;
	Vector p0ToOrigin = ray.origin - p0;
	double u = f * Vector::DotProduct(p0ToOrigin, dirCrossE1);
	//No intersection
	if (u < 0.0 || u > 1.0) return;

	auto originCrossE0 = Vector::CrossProduct(p0ToOrigin, e0);
	double v = f * Vector::DotProduct(ray.direction, originCrossE0);
	//No intersection
	if (v < 0.0 || u + v > 1.0) return;

	double t = f * Vector::DotProduct(e1, originCrossE0);

	//Ray intersects triangle
	buffer.Add(Intersection(t, GetPointer()));
}