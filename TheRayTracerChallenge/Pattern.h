#pragma once

#include "Color.h"
#include "Tuple.h"
#include "Transform.h"
#include <memory>

class Pattern
{
public:
	Pattern() = default;
	~Pattern() = default;

	Color ColorAtPoint(Point point, Transform shapeTransform) {
		Point objectPoint = point * shapeTransform.Inversion();
		Point patternPoint = objectPoint * GetTransform().Inversion();

		return ReadPattern(patternPoint);
	}

	void SetTransform(Transform t) { transform = t; }
	Transform GetTransform() { return transform; }


private:
	Transform transform;

	virtual Color ReadPattern(Point point) = 0;
};

