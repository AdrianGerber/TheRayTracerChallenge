#include "Sphere.h"



Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

IntersectionBuffer Sphere::FindObjectSpaceIntersections(Ray ray) {
	//Vector from the sphere's center to the ray's origin (Sphere centered at 0/0/0)
	Vector sphereToRayOrigin = ray.origin - Point::CreatePoint(0.0, 0.0, 0.0);

	//Calculating the discriminant
	double a = Vector::DotProduct(ray.direction, ray.direction);
	double b = 2.0 * Vector::DotProduct(ray.direction, sphereToRayOrigin);
	double c = Vector::DotProduct(sphereToRayOrigin, sphereToRayOrigin) - 1.0;

	double discriminant = (b * b) - (4.0 * a * c);


	//No intersections
	if (discriminant < 0) {
		return IntersectionBuffer();
	}

	Intersection i1, i2;
	i1.t = (-b - sqrt(discriminant)) / (2.0 * a);
	i2.t = (-b + sqrt(discriminant)) / (2.0 * a);

	i1.objectID = GetID();
	i2.objectID = GetID();

	return IntersectionBuffer(i1, i2);
}

Vector Sphere::FindObjectSpaceNormal(Point p) {
	return p - Point::CreatePoint(0.0, 0.0, 0.0);
}

