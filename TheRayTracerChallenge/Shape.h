#pragma once
#include "Transform.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Matrix.h"


class Shape
{
public:

	Shape() { material = Material(); id = Intersection::invalidID; transform.matrix = Matrix::IndentityMatrix4x4(); }
    ~Shape() = default;

    void SetID(size_t newID) { id = newID; }
    size_t GetID() { return id; }

    void SetTransform(Transform newTransform) { transform = newTransform; }
    Transform GetTransform() { return transform; }

    void SetMaterial(Material newMaterial) { material = newMaterial; }
    Material GetMaterial() { return material; }

	IntersectionBuffer FindIntersections(Ray ray) {
		//Transform the ray into object space
		Ray objectSpaceRay = ray.Transform(GetTransform().Inversion());

		//Find intersections
		return FindObjectSpaceIntersections(objectSpaceRay);
	}

	Vector SurfaceNormal(Point p) {
		//Point in object space
		Point objectSpacePoint = GetTransform().Inversion() * p;
		
		//Calculate normal
		Vector objectSpaceNormal = FindObjectSpaceNormal(objectSpacePoint);

		//Convert back to world space
		Vector worldSpaceNormal = GetTransform().Inversion().matrix.Transpose() * objectSpaceNormal;

		//Force worldSpaceNormal to be a vector
		worldSpaceNormal.w = 0.0;

		return worldSpaceNormal.Normalize();
	}

    bool operator==(Shape& s) {
        return
            GetTransform() == s.GetTransform()
            && GetMaterial() == s.GetMaterial()
            && GetID() == s.GetID();
    }

    bool operator!=(Shape& s) { return !(*this == s); }

	virtual IntersectionBuffer FindObjectSpaceIntersections(Ray ray) = 0;

	virtual Vector FindObjectSpaceNormal(Point p) = 0;

private:

    size_t id;
    Transform transform;
    Material material;
};

