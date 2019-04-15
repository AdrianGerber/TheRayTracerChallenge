#pragma once

#include "Ray.h"
#include "IntersectionBuffer.h"
#include "Shape.h"
#include "Tuple"
#include <vector>
#include <memory>
#include "Constants.h"

class HitCalculations {
public:
	double t;
	size_t shapeID;
	Point point;
	Point overPoint;
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

		//Slightly adjusted point, so it is on the correct side of the shapes
		//surface, even after floating point rounding errors
		overPoint = point + (normalVector * Constants::EPSILON);
	}
};