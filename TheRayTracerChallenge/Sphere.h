#pragma once

#include "Transform.h"

class Sphere
{
public:
    

    Sphere();
    ~Sphere();

    void SetTransform(Transform t);
    Transform GetTransform();

private:
    Transform transform;
};

inline void Sphere::SetTransform(Transform t) {
    transform = t;
}

inline Transform Sphere::GetTransform() {
    return transform;
}