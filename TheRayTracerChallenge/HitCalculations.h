#pragma once

#include "Ray.h"
#include "IntersectionBuffer.h"
#include "Shape.h"
#include "Tuple"
#include <vector>
#include <memory>

class HitCalculations {
public:
	float t;
	size_t shapeID;
	Point point;
	Vector eyeVector;
	Vector normalVector;
	bool insideShape;

	HitCalculations(IntersectionBuffer& intersections, const Ray& ray, std::vector<std::shared_ptr<Shape>>& shapes) {
		auto& hit = intersections.GetFirstHit();

		t = hit.t;
		shapeID = hit.objectID;
		point = ray.PositionAt(t);
		eyeVector = - ray.direction;
		normalVector = shapes[shapeID]->SurfaceNormal(point);

		//Flip the normal vector if it points away from the eye vector
		if (Vector::DotProduct(normalVector, eyeVector) < 0.0f) {
			insideShape = true;
			normalVector = -normalVector;
		}
		else {
			insideShape = false;
		}
	}
};