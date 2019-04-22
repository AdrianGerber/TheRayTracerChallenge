#pragma once
#include "Transform.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Matrix.h"

//Abstract class common to all shapes
class Shape
{
public:
	Shape() { material = Material(); id = Intersection::invalidID; }
    ~Shape() = default;

    void SetID(size_t newID) { id = newID; }
    size_t GetID() { return id; }

    void SetTransform(Transform newTransform) { transform = newTransform; }
	Transform GetTransformCopy() { return transform; }
	//Get the shape's transform by reference (Caching of calculations can improve performance
    Transform& GetTransformRef() { return transform; }



    void SetMaterial(Material newMaterial) { material = newMaterial; }
    Material GetMaterial() { return material; }

	//Find intersections of this shape and a ray
	IntersectionBuffer FindIntersections(Ray ray);

	//Calculate the surface normal at a point on the shape (Point assumed to be on shape's surface)
	Vector SurfaceNormal(Point p);

	//Calculate all intersections with a ray in object space (Individually implemented by each shape)
	virtual IntersectionBuffer FindObjectSpaceIntersections(Ray ray) = 0;

	//Calculate the normal vector of a point in object space (Implemented by each concrete shape)
	virtual Vector FindObjectSpaceNormal(Point p) = 0;


	//Comparison of shapes
	bool operator==(Shape& s);

    bool operator!=(Shape& s) { return !(*this == s); }

private:
    size_t id;
    Transform transform;
    Material material;
};


inline IntersectionBuffer Shape::FindIntersections(Ray ray) {
	//Transform the ray into object space
	Ray objectSpaceRay = ray.Transform(GetTransformRef().Inversion());

	//Find intersections
	return FindObjectSpaceIntersections(objectSpaceRay);
}

inline Vector Shape::SurfaceNormal(Point p) {
	//Point in object space
	Point objectSpacePoint = GetTransformRef().Inversion() * p;

	//Calculate normal
	Vector objectSpaceNormal = FindObjectSpaceNormal(objectSpacePoint);

	//Convert back to world space
	Vector worldSpaceNormal = GetTransformRef().Inversion().GetMatrix().Transpose() * objectSpaceNormal;

	//Force worldSpaceNormal to be a vector
	worldSpaceNormal.w = 0.0;

	return worldSpaceNormal.Normalize();
}

inline bool Shape::operator==(Shape& s) {
	return
		GetTransformRef() == s.GetTransformRef()
		&& GetMaterial() == s.GetMaterial()
		&& GetID() == s.GetID();
}