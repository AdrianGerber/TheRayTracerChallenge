#pragma once

#include "Transform.h"
#include "Tuple.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Shape.h"

//A sphere of radius 1 (unless transformed)
class ShapeGroup : public Shape
{
public:
	ShapeGroup() = default;
	~ShapeGroup() = default;

	//Virtual methods that need to be implemented
	IntersectionBuffer FindObjectSpaceIntersections(Ray ray) override;

	Vector FindObjectSpaceNormal(Point p) override;

	size_t GetShapeCount() { return shapes.size(); }
	std::shared_ptr<Shape> GetShape(size_t index) { return shapes[index]; }
	void AddShape(std::shared_ptr<Shape> shape) { shapes.push_back(shape); shape->SetParent(GetPointer()); }
	bool ContainsShape(std::shared_ptr<Shape> shape) { return std::find(shapes.begin(), shapes.end(), shape) != shapes.end(); }


private:
	std::vector<std::shared_ptr<Shape>> shapes;

	std::shared_ptr<Shape> ShapeSpecificCopy() override {
		auto group = Shape::MakeShared<ShapeGroup>();

		for (auto shape : shapes) {
			auto copy = shape->Copy();
			group->AddShape(copy);
		}

		return group;
	}
};

inline IntersectionBuffer ShapeGroup::FindObjectSpaceIntersections(Ray ray) {
	IntersectionBuffer intersections;


	//Find all intersections inside this group of shapes
	for (auto currentShape : shapes) {
		intersections.Add(currentShape->FindIntersections(ray));
	}

	return intersections;
}

inline Vector ShapeGroup::FindObjectSpaceNormal(Point p) {
	std::cerr << "Normal of Group\n";
	return Vector::CreateVector(0.0, 0.0, 0.0);
}

