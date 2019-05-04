#pragma once

#include "Transform.h"
#include "Tuple.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Shape.h"

//A Group of multiple shapes
class ShapeGroup : public Shape
{
public:
	ShapeGroup() = default;
	~ShapeGroup() = default;

	//Virtual methods that need to be implemented
	void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

	Vector FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) override;

	size_t GetShapeCount() { return shapes.size(); }
	std::shared_ptr<Shape> GetShape(size_t index) { return shapes[index]; }
	void AddShape(std::shared_ptr<Shape> shape) {
		shapes.push_back(shape); shape->SetParent(GetPointer());
		bounds.Add(shape->GetParentSpaceBounds());
	}

	BoundingBox GetObjectSpaceBounds() override { return bounds; }

	void PartitionChildren(size_t maximumShapeCount) override;

	void SetMaterial(Material newMaterial) override;

	bool ContainsShape(std::shared_ptr<Shape> shape) override {
		for (auto currentShape : shapes) {
			if (currentShape->ContainsShape(shape)) {
				return true;
			}
		}

		return false;
	}

private:
	std::vector<std::shared_ptr<Shape>> shapes;

	BoundingBox bounds;

	std::shared_ptr<Shape> ShapeSpecificCopy() override {
		auto group = Shape::MakeShared<ShapeGroup>();

		for (auto shape : shapes) {
			auto copy = shape->Copy();
			group->AddShape(copy);
		}

		return group;
	}
};

inline void ShapeGroup::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) {
	IntersectionBuffer intersections;

	//The contained shapes only need to be checked when the bounding box is hit by the ray
	if (bounds.CheckIntersection(ray)) {
		//Find all intersections inside this group of shapes
		for (auto currentShape : shapes) {
			currentShape->FindIntersections(ray, buffer);
		}
	}
}

inline Vector ShapeGroup::FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) {
	std::cerr << "Normal of Group\n";
	return Vector::CreateVector(0.0, 0.0, 0.0);
}

inline void ShapeGroup::PartitionChildren(size_t maximumShapeCount) {

	//Only subdivide this group if the number of shapes is above the maximum
	if (GetShapeCount() > maximumShapeCount) {
		std::vector<std::shared_ptr<Shape>> keep;
		auto shapes1 = Shape::MakeShared<ShapeGroup>();
		auto shapes2 = Shape::MakeShared<ShapeGroup>();

		auto subgroupBounds = bounds.SplitBox();

		//Partition the shapes into two subgroups
		for (auto shape : shapes) {
			auto shapeBounds = shape->GetParentSpaceBounds();
			bool isInFirstGroup = subgroupBounds.first.Contains(shapeBounds.GetMin()) && subgroupBounds.first.Contains(shapeBounds.GetMax());
			bool isInSecondGroup = subgroupBounds.second.Contains(shapeBounds.GetMin()) && subgroupBounds.second.Contains(shapeBounds.GetMax());

			//Shape fits into the first subgroup
			if (isInFirstGroup && !isInSecondGroup) {
				shapes1->AddShape(shape);
			}
			//Shape fits into the seconds subgroup
			else if (isInSecondGroup) {
				shapes2->AddShape(shape);
			}
			//Shape doesn't fit in either of the groups
			else {
				keep.push_back(shape);
			}

		}

		//Remove all shapes from this group
		shapes.clear();
		//Only keep the shapes that don't fit in either one of the partitions
		for (auto shape : keep) {
			AddShape(shape);
			shape->PartitionChildren(maximumShapeCount);
		}

		//Keep the subgroups only if they aren't empty
		if (shapes1->GetShapeCount() != 0) {
			AddShape(shapes1);
			//Subdivide the subgroup
			shapes1->PartitionChildren(maximumShapeCount);
		}

		if (shapes2->GetShapeCount() != 0) {
			AddShape(shapes2);
			shapes2->PartitionChildren(maximumShapeCount);
		}
	}
	else {
		//Maybe one of the shapes is a group that can be subdivided
		for (auto shape : shapes) {
			shape->PartitionChildren(maximumShapeCount);
		}
	}
}


inline void ShapeGroup::SetMaterial(Material newMaterial) {
	material = newMaterial;

	for (auto shape : shapes) {
		shape->SetMaterial(material);
	}
}