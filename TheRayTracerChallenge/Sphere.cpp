#include "Sphere.h"



Sphere::Sphere()
{
    material = Material();
}


Sphere::~Sphere()
{
}

Vector Sphere::SurfaceNormal(Point p) {
    Point objectSpacePoint = transform.Inversion() * p;
    Vector objectSpaceNormal = objectSpacePoint - Point::CreatePoint(0.0f, 0.0f, 0.0f);
    Vector worldSpaceNormal = transform.Inversion().matrix.Transpose() * objectSpaceNormal;

    //Force worldSpaceNormal to be a vector
    worldSpaceNormal.w = 0.0f;

    return worldSpaceNormal.Normalize();
}