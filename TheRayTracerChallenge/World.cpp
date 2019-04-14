#include "World.h"


World::World()
{
}


World::~World()
{
}

void World::LoadDefaultWorld()
{
    lightSources.clear();
    shapes.clear();

    //Create default light source    
    LightSource l(Point::CreatePoint(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 1.0f));
    lightSources.push_back(std::make_shared<LightSource>());
    *lightSources.back() = l;

    //Create default spheres
    Material m;
    m.color = Color(0.8f, 1.0f, 0.6f);
    m.diffuse = 0.7f;
    m.specular = 0.2f;
    
    Sphere s1;
    s1.SetMaterial(m);
    shapes.push_back(std::make_shared<Sphere>());
    *shapes.back() = s1;

    Sphere s2;
    s2.SetTransform(Transform::CreateScale(0.5f, 0.5f, 0.5f));
    shapes.push_back(std::make_shared<Sphere>());
    *shapes.back() = s2;
}

IntersectionBuffer World::IntersectRay(Ray ray)
{
    IntersectionBuffer allIntersections;


    for (auto currentShape : shapes) {
        allIntersections.Add(currentShape->FindIntersections(ray));
    }

    allIntersections.Sort();

    return allIntersections;
}