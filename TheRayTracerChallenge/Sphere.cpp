#include "Sphere.h"



Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

Vector Sphere::SurfaceNormal(Point p) {
    Point objectSpacePoint = GetTransform().Inversion() * p;
    Vector objectSpaceNormal = objectSpacePoint - Point::CreatePoint(0.0f, 0.0f, 0.0f);
    Vector worldSpaceNormal = GetTransform().Inversion().matrix.Transpose() * objectSpaceNormal;

    //Force worldSpaceNormal to be a vector
    worldSpaceNormal.w = 0.0f;

    return worldSpaceNormal.Normalize();
}


IntersectionBuffer Sphere::FindIntersections(Ray ray)
{
    Ray objectSpaceRay = ray.Transform(GetTransform().Inversion());


    //Vector from the sphere's center to the ray's origin (Sphere centered at 0/0/0)
    Vector sphereToRayOrigin = objectSpaceRay.origin - Point::CreatePoint(0.0f, 0.0f, 0.0f);

    //Calculating the discriminant
    float a = Vector::DotProduct(objectSpaceRay.direction, objectSpaceRay.direction);
    float b = 2.0f * Vector::DotProduct(objectSpaceRay.direction, sphereToRayOrigin);
    float c = Vector::DotProduct(sphereToRayOrigin, sphereToRayOrigin) - 1.0f;

    float discriminant = (b*b) - (4.0f * a * c);


    //No intersections
    if (discriminant < 0) {
        return IntersectionBuffer();
    }

    Intersection i1, i2;
    i1.t = (-b - sqrtf(discriminant)) / (2.0f * a);
    i2.t = (-b + sqrtf(discriminant)) / (2.0f * a);

    i1.objectID = GetID();
    i2.objectID = GetID();

    return IntersectionBuffer(i1, i2);
}
