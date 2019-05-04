#pragma once

#include "Color.h"
#include "Tuple.h"
#include "Transform.h"
#include <memory>

namespace RayTracer {
	class Shape;

	//Abstract class describing patterns
	class Pattern
	{
	public:
		Pattern() = default;
		~Pattern() = default;

		//Find the pattern's color at a point in world space
		Color ColorAtShapePoint(Point shapePoint);

		void SetTransform(Transform t) { transform = t; }
		Transform GetTransform() { return transform; }


		std::shared_ptr<Pattern> Copy() {
			auto copy = PatternSpecificCopy();
			copy->SetTransform(transform);
			return copy;
		}

	private:
		Transform transform;

		//Method used to determine the color at a local position
		virtual Color ReadPattern(Point point) = 0;

		virtual std::shared_ptr<Pattern> PatternSpecificCopy() = 0;
	};

	inline Color Pattern::ColorAtShapePoint(Point shapePoint) {
		//Transform the point to the pattern's coordinate space
		Point patternPoint = shapePoint * GetTransform().Inversion();
		return ReadPattern(patternPoint);
	}
}
