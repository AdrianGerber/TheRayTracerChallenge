#pragma once

#include "Transform.h"
#include "Tuple.h"
#include "Material.h"
#include "IntersectionBuffer.h"
#include "Ray.h"
#include "Shape.h"

class Sphere : public Shape
{
public:

    Sphere();
    ~Sphere();

    Vector SurfaceNormal(Point p);

    IntersectionBuffer FindIntersections(Ray ray) override;
    
private:
};

