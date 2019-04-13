#pragma once

#include "Transform.h"
#include "Tuple.h"
#include "Material.h"

class Sphere
{
public:

    Sphere();
    ~Sphere();

    void SetTransform(Transform t) { this->transform = t; }
    Transform GetTransform() { return transform; }

    Vector SurfaceNormal(Point p);

    void SetMaterial(Material m) { this->material = m; }
    Material GetMaterial() { return material; }

    size_t GetID() { return id; };
    void SetID(size_t id) { this->id = id; }

    bool operator==(Sphere s) {
        return
            transform == s.GetTransform()
            && material == s.GetMaterial()
            && id == s.GetID();
    }

    bool operator!=(Sphere s) { return !(*this == s); }


private:
    Transform transform;
    Material material;
    size_t id;  

};

