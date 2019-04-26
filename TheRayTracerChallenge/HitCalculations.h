#pragma once

#include "Ray.h"
#include "IntersectionBuffer.h"
#include "Shape.h"
#include "Tuple"
#include <vector>
#include <memory>
#include <algorithm>
#include "Constants.h"

//Calculates attributes that further describe the intersection of a ray with a shape
class HitCalculations {
public:
	double t;
	std::shared_ptr<Shape> shape;
	Point point;
	Point overPoint;
	Point underPoint;
	Vector eyeVector;
	Vector normalVector;
	Vector reflectionVector;

	double refractiveIndex1;
	double refractiveIndex2;

	bool insideShape;

	HitCalculations(const Intersection& intersection, IntersectionBuffer& intersections, const Ray& ray, std::vector<std::shared_ptr<Shape>>& shapes) {

		t = intersection.t;
		shape = intersection.shape;
		point = ray.PositionAt(t);
		eyeVector = - ray.direction;
		normalVector = shape->SurfaceNormal(point);

		//Flip the normal vector if it points away from the eye vector
		if (Vector::DotProduct(normalVector, eyeVector) < 0.0) {
			insideShape = true;
			normalVector = -normalVector;
		}
		else {
			insideShape = false;
		}


		reflectionVector = ray.direction.Reflect(normalVector);

		//Slightly adjusted points slightly above and below the actual hit:
		//These are used for shading and reflection, so the reflected ray doesn't
		//collide with the original shape due to floating point inaccuracies
		overPoint = point + (normalVector * Constants::EPSILON);
		underPoint = point - (normalVector * Constants::EPSILON);


		//List of objects that the ray is currently inside of
		std::vector<std::shared_ptr<Shape>> containers;


		intersections.Sort();


		//Refractive indices of the materials at the 'intersection'
		//Default values
		refractiveIndex1 = 1.0;
		refractiveIndex2 = 1.0;

		//Go through every intersection
		size_t numberOfIntersections = intersections.GetCount();
		for (size_t index = 0; index < numberOfIntersections; index++) {
			Intersection currentIntersection = intersections[index];

			//Specified intersection was reached
			if (intersection == currentIntersection) {
				//Find the refractive index of the first material
				if (containers.empty()) {
					refractiveIndex1 = 1.0;
				}
				else {
					refractiveIndex1 = containers.back()->GetMaterial().refractiveIndex;
				}
			}


			//Keep track of the objects that the ray is currently inside of
			auto objectPosition = std::find(containers.begin(), containers.end(), currentIntersection.shape);
			//The current object is already inside the list
			if (objectPosition != containers.end()) {
				//The ray leaves the shape at this intersection -> remove from containers
				containers.erase(objectPosition);
			}
			//Object is not in the list
			else {
				//The ray enters the object at this intersection -> Add to container
				containers.push_back(currentIntersection.shape);
			}

			//Specified intersection was reached
			if (intersection == currentIntersection) {
				//Find the refractive index of the second material
				if (containers.empty()) {
					refractiveIndex2 = 1.0;
				}
				else {
					refractiveIndex2 = containers.back()->GetMaterial().refractiveIndex;
				}

				break;
			}
		}
	}

	//Approximation used to blend the intensities of reflected and refracted colors (Simulates the 'Fresnel Effect')
	double SchlickApproximation() const {
		double cos = Vector::DotProduct(eyeVector, normalVector);

		//Total internal reflection can only occur if N1 > N2
		if (refractiveIndex1 > refractiveIndex2) {
			double refractiveRatio = refractiveIndex1 / refractiveIndex2;
			double sin2_t = refractiveRatio * refractiveRatio * (1.0 - (cos * cos));

			//Total internal reflection
			if (sin2_t > 1.0) {
				return 1.0;
			}

			//When n1 > n2, use cos(theta_t) instead of cos for the other calculations
			cos = sqrt(1.0 - sin2_t);
		}

		//Math...
		double r0 = pow((refractiveIndex1 - refractiveIndex2)
			/ (refractiveIndex1 + refractiveIndex2), 2.0);

		return r0 + ((1 - r0) * pow(1 - cos, 5.0));
	}
};