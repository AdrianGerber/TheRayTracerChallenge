#pragma once

#include "Tuple.h"
#include "Sphere.h"
#include "Intersection.h"
#include <cmath>
#include <memory>
#include "Transform.h"

class Ray
{
public:
    Point origin;
    Vector direction;

    //Constructors and destructors
    Ray();
    Ray(Point origin, Vector direction);
    ~Ray();

    //Ray tracing
    IntersectionBuffer FindIntersections(std::shared_ptr<Sphere> sphere);
    
    //Mathematical functions
    Point PositionAt(float t);

    Ray Transform(Transform m);

};


//Constructors and destructors
inline Ray::Ray()
{
}

inline Ray::Ray(Point origin, Vector direction) {
    this->origin = origin;
    this->direction = direction;
}

inline Ray::~Ray()
{
}


//Ray tracing

inline IntersectionBuffer Ray::FindIntersections(std::shared_ptr<Sphere> sphere) {
    Ray objectSpaceRay = Ray::Transform(sphere->GetTransform().Inversion());


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
    
    i1.objectID = sphere->GetID();
    i2.objectID = sphere->GetID();
   
    return IntersectionBuffer(i1, i2);
}



//Mathematical functions

inline Point Ray::PositionAt(float t) {
    return origin + (direction * t);
}

inline Ray Ray::Transform(::Transform m) {
    Ray r;
    r.direction = m * direction;
    r.origin = m * origin;
    return r;
}