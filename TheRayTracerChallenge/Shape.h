#pragma once
#include "Transform.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"

class Shape
{
public:

    Shape(){ SetMaterial(Material()); }
    ~Shape() = default;

    void SetID(size_t newID) { id = newID; }
    size_t GetID() { return id; }

    void SetTransform(Transform newTransform) { transform = newTransform; }
    Transform GetTransform() { return transform; }

    void SetMaterial(Material newMaterial) { material = newMaterial; }
    Material GetMaterial() { return material; }

    virtual IntersectionBuffer FindIntersections(Ray ray) = 0;

    bool operator==(Shape& s) {
        return
            GetTransform() == s.GetTransform()
            && GetMaterial() == s.GetMaterial()
            && GetID() == s.GetID();
    }

    bool operator!=(Shape& s) { return !(*this == s); }


private:
    size_t id;
    Transform transform;
    Material material;
};

