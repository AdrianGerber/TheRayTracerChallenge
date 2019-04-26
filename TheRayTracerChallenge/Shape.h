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
	Shape() { material = Material(); }
    ~Shape() = default;

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


	//Check if both shapes have the same pointer
	bool operator==(std::shared_ptr<Shape> s);
	//Shapes are not the same pointer, but have the same transform
	bool SameTransform(std::shared_ptr<Shape> s);



	//Create an initialized instance of a shape
	template<typename T, typename ...Args>
	static std::shared_ptr<T> MakeShared(Args&& ...args) {
		auto shape = std::make_shared<T>(std::forward<Args>(args)...);
		shape->SetPointer(shape);
		return shape;
	}

	std::shared_ptr<Shape> GetPointer() { return static_cast<std::shared_ptr<Shape>>(thisShapePtr); }
	void SetPointer(std::shared_ptr<Shape> newPtr) { thisShapePtr = static_cast<std::weak_ptr<Shape>>(newPtr); }


private:
    Transform transform;
    Material material;
	std::weak_ptr<Shape> thisShapePtr;
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


//Check if both shapes have the same pointer
inline bool Shape::operator==(std::shared_ptr<Shape> s) {
	return GetPointer() == s->GetPointer();
}

//Shapes are not the same pointer, but otherwise equal
inline bool Shape::SameTransform(std::shared_ptr<Shape> s) {
	return
		GetTransformRef() == s->GetTransformRef();
}
