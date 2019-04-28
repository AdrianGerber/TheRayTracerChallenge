#pragma once

#include "Shape.h"

class Triangle : public Shape {
public:

	void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override { return; }

	Vector FindObjectSpaceNormal(Point p) override { return Vector();}

	void PartitionChildren(size_t maximumShapeCount) override {}

	BoundingBox GetObjectSpaceBounds() override {
		/*
		function bounds_of(triangle)
  let box ? bounding_box(empty)

  add triangle.p1 to box
  add triangle.p2 to box
  add triangle.p3 to box

  return box
end function
*/



		return BoundingBox();
	}

private:
	std::shared_ptr<Shape> ShapeSpecificCopy() override {
		return Shape::MakeShared<Triangle>(*this);
	}
};