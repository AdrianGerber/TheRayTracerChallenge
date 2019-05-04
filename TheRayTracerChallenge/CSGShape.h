#pragma once
#include "Shape.h"
#include <memory>

namespace RayTracer {
	class CSGShape : public Shape {
	public:
		enum class Operation { Union, Difference, Intersection };

		CSGShape() { operation = Operation::Union; }
		~CSGShape() = default;

		void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) override;

		Vector FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) override;

		void PartitionChildren(size_t maximumShapeCount) override {}

		BoundingBox GetObjectSpaceBounds() override;

		void SetLeft(std::shared_ptr<Shape> leftShape) {
			left = leftShape; left->SetParent(GetPointer());
			RecalculateBoundingBox();
		}
		void SetRight(std::shared_ptr<Shape> rightShape) {
			right = rightShape; right->SetParent(GetPointer());
			RecalculateBoundingBox();
		}

		const std::shared_ptr<Shape> GetLeft() { return left; }
		const std::shared_ptr<Shape> GetRight() { return right; }

		void SetOperation(Operation newOperation) { operation = newOperation; }
		Operation GetOperation() { return operation; }

		static bool IntersectionAllowed(Operation operation, bool leftHit, bool inLeft, bool inRight);

		void FilterIntersections(IntersectionBuffer& in, IntersectionBuffer& out);

		bool ContainsShape(std::shared_ptr<Shape> shape) override {
			if (left->ContainsShape(shape) || right->ContainsShape(shape)) return true;
			return false;
		}
	private:
		std::shared_ptr<Shape> left, right;
		Operation operation;
		BoundingBox bounds;
		IntersectionBuffer reusedIntersectionBuffer;

		void RecalculateBoundingBox() {
			bounds = BoundingBox();

			if (left) {
				bounds.Add(left->GetParentSpaceBounds());
			}
			if (right) {
				bounds.Add(right->GetParentSpaceBounds());
			}
		}

		std::shared_ptr<Shape> ShapeSpecificCopy() override {
			auto leftCpy = left->Copy();
			auto rightCpy = right->Copy();
			auto csgShape = Shape::MakeShared<CSGShape>();
			csgShape->SetOperation(GetOperation());
			csgShape->SetLeft(leftCpy);
			csgShape->SetRight(rightCpy);

			return csgShape;
		}
	};

	inline bool CSGShape::IntersectionAllowed(Operation operation, bool leftHit, bool inLeft, bool inRight) {
		switch (operation)
		{
			//Shapes are combined
		case CSGShape::Operation::Union:
			return (leftHit && !inRight) || (!leftHit && !inLeft);
			break;

			//Shapes are subtracted from each other
		case CSGShape::Operation::Difference:
			return (leftHit && !inRight) || (!leftHit && inLeft);
			break;

			//Only the intersection of both shapes sould be rendered
		case CSGShape::Operation::Intersection:
			return (leftHit && inRight) || (!leftHit && inLeft);
			break;

		default:
			break;
		}

		return false;
	}

	inline void CSGShape::FilterIntersections(IntersectionBuffer& in, IntersectionBuffer& out) {
		//Start outside of both shapes
		bool inLeft = false, inRight = false;

		size_t intersectionCount = in.GetCount();
		in.Sort();

		for (size_t index = 0; index < intersectionCount; index++) {
			//Was the hit on the CSGShape's left element
			bool leftHit = left->ContainsShape(in[index].shape);

			//Filter the intersections
			if (IntersectionAllowed(operation, leftHit, inLeft, inRight)) {
				out.Add(in[index]);
			}

			//Keep track of what shapes the ray is currently inside of
			if (leftHit) {
				inLeft = !inLeft;
			}
			else {
				inRight = !inRight;
			}
		}
	}

	inline void CSGShape::FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) {
		reusedIntersectionBuffer.Reset();

		//Ray misses all subshapes
		if (!bounds.CheckIntersection(ray)) return;

		//Find all intersections
		right->FindIntersections(ray, reusedIntersectionBuffer);
		left->FindIntersections(ray, reusedIntersectionBuffer);

		reusedIntersectionBuffer.Sort();

		FilterIntersections(reusedIntersectionBuffer, buffer);

	}

	inline Vector CSGShape::FindObjectSpaceNormal(Point p, const Intersection& globalIntersection) {
		//Should never be called
		std::cout << "Normal on CSG operation";
		return Vector();
	}

	inline BoundingBox CSGShape::GetObjectSpaceBounds() {
		return bounds;
	}
}