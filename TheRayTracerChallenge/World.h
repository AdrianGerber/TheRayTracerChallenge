#pragma once
#include "Sphere.h"
#include "Sphere.h"
#include "LightSource.h"
#include <memory>
#include <vector>
#include "Ray.h"

class World
{
public:
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<LightSource>> lightSources;

    World();
    ~World();


    void LoadDefaultWorld();
    IntersectionBuffer IntersectRay(Ray ray);
};

