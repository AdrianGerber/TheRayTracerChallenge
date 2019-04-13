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

private:
    Transform transform;
    Material material;

};

