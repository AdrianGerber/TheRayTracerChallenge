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
	shapes.back()->SetID(shapes.size() - 1);

    Sphere s2;
    s2.SetTransform(Transform::CreateScale(0.5f, 0.5f, 0.5f));
    shapes.push_back(std::make_shared<Sphere>());
    *shapes.back() = s2;
	shapes.back()->SetID(shapes.size() - 1);
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

Color World::ShadeHit(const HitCalculations& hitInfo)
{
	Color resultingColor(0.0f, 0.0f, 0.0f);

	for (auto& lightSource : lightSources) {
		bool inShadow = PointIsInShadow(lightSource, hitInfo.overPoint);
		resultingColor = resultingColor + lightSource->Lighting(shapes[hitInfo.shapeID]->GetMaterial(), hitInfo.overPoint, hitInfo.eyeVector, hitInfo.normalVector, inShadow);
	}

	return resultingColor;
}

bool World::PointIsInShadow(std::shared_ptr<LightSource> lightSource, Point point)
{
	Vector vectorToLightSource = lightSource->GetPosition() - point;
	double distanceToLightSource = vectorToLightSource.Magnitude();

	//Ray from the point to the light source
	Ray ray(point, vectorToLightSource.Normalize());

	IntersectionBuffer intersections = IntersectRay(ray);
	Intersection hit = intersections.GetFirstHit();

	//Is anything blocking the light from reaching the point (-> object between point and light source)?
	if (hit.IsValid() && hit.t < distanceToLightSource) {
		//Point is in shadow
		return true;
	}

	//Point is illuminated by the light source
	return false;
}

Color World::FindRayColor(Ray ray)
{
	IntersectionBuffer intersections = IntersectRay(ray);

	if (intersections.GetCount() == 0 || !intersections.GetFirstHit().IsValid()) return Color(0.0f, 0.0f, 0.0f);

	return ShadeHit(HitCalculations(intersections, ray, shapes));
}
