#pragma once
#include "Transform.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Matrix.h"
#include "BoundingBox.h"

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
	void FindIntersections(Ray ray, IntersectionBuffer& buffer);

	//Calculate the surface normal at a point on the shape (Point assumed to be on shape's surface)
	Vector SurfaceNormal(Point p);

	//Calculate all intersections with a ray in object space (Individually implemented by each shape)
	virtual void FindObjectSpaceIntersections(Ray ray, IntersectionBuffer& buffer) = 0;

	//Calculate the normal vector of a point in object space (Implemented by each concrete shape)
	virtual Vector FindObjectSpaceNormal(Point p) = 0;


	//Check if both shapes have the same pointer
	bool operator==(std::shared_ptr<Shape> s);
	//Shapes are not the same pointer, but have the same transform
	bool SameTransform(std::shared_ptr<Shape> s);

	//Take transforms of parent shapes into consideration
	Point PointToObjectSpace(Point p);
	Vector NormalToWorldSpace(Vector normal);

	//Create an initialized instance of a shape
	template<typename T, typename ...Args>
	static std::shared_ptr<T> MakeShared(Args&& ...args) {
		auto shape = std::make_shared<T>(std::forward<Args>(args)...);
		shape->SetPointer(shape);
		shape->SetParent(nullptr);
		return shape;
	}

	std::shared_ptr<Shape> GetPointer() { return static_cast<std::shared_ptr<Shape>>(thisShapePtr); }
	void SetPointer(std::shared_ptr<Shape> newPtr) { thisShapePtr = static_cast<std::weak_ptr<Shape>>(newPtr); }

	std::shared_ptr<Shape> GetParent() {
		if (parent.expired()) return std::shared_ptr<Shape>();
		return static_cast<std::shared_ptr<Shape>>(parent);
	}
	void SetParent(std::shared_ptr<Shape> newPtr) { parent = static_cast<std::weak_ptr<Shape>>(newPtr); }

	std::shared_ptr<Shape> Copy() {
		auto shape = ShapeSpecificCopy();
		shape->SetMaterial(material.Copy());
		shape->SetParent(GetParent());
		shape->SetTransform(GetTransformCopy());
		return shape;
	}

	BoundingBox GetParentSpaceBounds() { return GetObjectSpaceBounds().ApplyTransform(transform); }

	virtual BoundingBox GetObjectSpaceBounds() = 0;

	virtual void PartitionChildren(size_t maximumShapeCount) = 0;

private:

	virtual std::shared_ptr<Shape> ShapeSpecificCopy() = 0;

	Transform transform;
	Material material;
	std::weak_ptr<Shape> thisShapePtr;
	std::weak_ptr<Shape> parent;
};


inline void Shape::FindIntersections(Ray ray, IntersectionBuffer& buffer) {
	//Transform the ray into object space
	Ray objectSpaceRay = ray.Transform(GetTransformRef().Inversion());

	//Find intersections
	FindObjectSpaceIntersections(objectSpaceRay, buffer);
}

inline Vector Shape::SurfaceNormal(Point p) {
	//Point in object space
	Point objectSpacePoint = PointToObjectSpace(p);

	//Calculate normal
	Vector objectSpaceNormal = FindObjectSpaceNormal(objectSpacePoint);

	//Convert back to world space
	return NormalToWorldSpace(objectSpaceNormal);
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


inline Point Shape::PointToObjectSpace(Point p) {
	//Handle the transforms of parent shapes
	if (!parent.expired()) {
		p = GetParent()->PointToObjectSpace(p);
	}

	return transform.Inversion() * p;
}

inline Vector Shape::NormalToWorldSpace(Vector normal) {
	normal = transform.Inversion().GetMatrix().Transpose() * normal;
	normal.w = 0.0;
	normal = normal.Normalize();

	//Handle parent transforms
	if (!parent.expired()) {
		normal = GetParent()->NormalToWorldSpace(normal);
	}

	return normal;
}