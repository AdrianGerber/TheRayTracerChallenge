#pragma once

#include "Color.h"
#include "Tuple.h"
#include "Transform.h"
#include <memory>

//Abstract class describing patterns
class Pattern
{
public:
	Pattern() = default;
	~Pattern() = default;

	//Find the pattern's color at a point in world space
	Color ColorAtPoint(Point point, Transform shapeTransform);

	void SetTransform(Transform t) { transform = t; }
	Transform GetTransform() { return transform; }


private:
	Transform transform;

	//Method used to determine the color at a local position
	virtual Color ReadPattern(Point point) = 0;
};

inline Color Pattern::ColorAtPoint(Point point, Transform shapeTransform) {
	//Transform the point to the pattern's coordinate space
	Point objectPoint = point * shapeTransform.Inversion();
	Point patternPoint = objectPoint * GetTransform().Inversion();

	return ReadPattern(patternPoint);
}