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
    LightSource l(Point::CreatePoint(-10.0, 10.0, -10.0), Color(1.0, 1.0, 1.0));
    lightSources.push_back(std::make_shared<LightSource>());
    *lightSources.back() = l;

    //Create default spheres
    Material m;
	auto color = std::make_shared<ColorPattern>();
	color->SetColor(Color(0.8, 1.0, 0.6));
	m.pattern = color;
    m.diffuse = 0.7;
    m.specular = 0.2;
    
    Sphere s1;
    s1.SetMaterial(m);
    shapes.push_back(std::make_shared<Sphere>());
    *shapes.back() = s1;
	shapes.back()->SetID(shapes.size() - 1);

    Sphere s2;
    s2.SetTransform(Transform::CreateScale(0.5, 0.5, 0.5));
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

Color World::ShadeHit(const HitCalculations& hitInfo, size_t remainingReflections)
{
	Color lightingColor(0.0, 0.0, 0.0);
	
	for (auto& lightSource : lightSources) {
		bool inShadow = PointIsInShadow(lightSource, hitInfo.overPoint);
		lightingColor = lightingColor + lightSource->Lighting(shapes[hitInfo.shapeID], hitInfo.overPoint, hitInfo.eyeVector, hitInfo.normalVector, inShadow);
	}


	Color reflectedColor = FindReflectedColor(hitInfo, remainingReflections);
	Color refractedColor = FindRefractedColor(hitInfo, remainingReflections);

	Material shapeMaterial = shapes[hitInfo.shapeID]->GetMaterial();

	//Material is both reflective and transparent -> use Schlick Approximation
	if (shapeMaterial.reflective > 0.0 && shapeMaterial.transparency > 0.0) {
		double reflectance = hitInfo.SchlickApproximation();

		return lightingColor
			+ (reflectedColor * reflectance)
			+ (refractedColor * (1.0 - reflectance));
	}


	return lightingColor + reflectedColor + refractedColor;
}

Color World::FindReflectedColor(const HitCalculations& hitInfo, size_t remainingReflections)
{
	Material material = shapes[hitInfo.shapeID]->GetMaterial();

	//Material is not reflective
	if (Constants::DoubleEqual(material.reflective, 0.0)) {
		return Color(0.0, 0.0, 0.0);
	}

	//Maximum count of reflections is reached
	if (remainingReflections == 0) {
		return Color(0.0, 0.0, 0.0);
	}

	Ray reflectedRay(hitInfo.overPoint, hitInfo.reflectionVector);

	Color hitColor = FindRayColor(reflectedRay, remainingReflections - 1);

	return hitColor * material.reflective;
}

Color World::FindRefractedColor(const HitCalculations& hitInfo, size_t remainingRefractions)
{
	//Maximum number of refractions reached
	if (remainingRefractions == 0) {
		return Color(0.0, 0.0, 0.0);
	}

	Material m = shapes[hitInfo.shapeID]->GetMaterial();

	//Material not transparent -> black
	if (Constants::DoubleEqual(m.transparency, 0.0)) {
		return Color(0.0, 0.0, 0.0);
	}

	//Ratio between refraction indices
	double refractionRatio = hitInfo.refractiveIndex1 / hitInfo.refractiveIndex2;

	//Cos(theta_i) is the same as the dot product of these vectors (Derived from Snell's Law)
	double cos_i = Vector::DotProduct(hitInfo.eyeVector, hitInfo.normalVector);
	
	//Find sin(theta_t)^2
	double sin2_t = (refractionRatio * refractionRatio) * (1.0 - (cos_i * cos_i));

	//Total internal reflection?
	if (sin2_t > 1.0) {
		return Color(0.0, 0.0, 0.0);
	}

	double cos_t = sqrt(1.0 - sin2_t);

	//Compute the direction of the refracted ray
	Vector refractedDirection = hitInfo.normalVector * (refractionRatio * cos_i - cos_t)
		- (hitInfo.eyeVector * refractionRatio);

	Ray refractedRay(hitInfo.underPoint, refractedDirection);

	Color color = FindRayColor(refractedRay, remainingRefractions - 1) * m.transparency;

	return color;
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

Color World::FindRayColor(Ray ray, size_t remainingReflections)
{
	IntersectionBuffer intersections = IntersectRay(ray);

	if (intersections.GetCount() == 0 || !intersections.GetFirstHit().IsValid()) return Color(0.0, 0.0, 0.0);

	return ShadeHit(HitCalculations(intersections.GetFirstHit(), intersections, ray, shapes), remainingReflections);
}
