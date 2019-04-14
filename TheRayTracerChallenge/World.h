#pragma once
#include "Sphere.h"
#include "Sphere.h"
#include "LightSource.h"
#include <memory>
#include <vector>
#include "Ray.h"
#include "HitCalculations.h"

class World
{
public:
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<LightSource>> lightSources;

    World();
    ~World();


    void LoadDefaultWorld();
    IntersectionBuffer IntersectRay(Ray ray);
	Color ShadeHit(const HitCalculations& hitInfo);

	void AddShape(std::shared_ptr<Shape> shape) {
		shapes.push_back(shape);
		shapes.back()->SetID(shapes.size() - 1);
	}

	void AddLightSource(std::shared_ptr<LightSource> lightSource) {
		lightSources.push_back(lightSource);
	}

	Color FindRayColor(Ray ray);
};

