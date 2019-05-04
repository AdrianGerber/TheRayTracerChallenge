#include "LightSource.h"



RayTracer::LightSource::LightSource(Point position, Color intensity)
{
    SetIntensity(intensity);
    SetPosition(position);
}

RayTracer::LightSource::LightSource()
{
    SetIntensity(Color(1.0, 1.0, 1.0));
    SetPosition(Point::CreatePoint(0.0, 0.0, 0.0));
}


RayTracer::LightSource::~LightSource()
{
}

RayTracer::Color RayTracer::LightSource::Lighting(std::shared_ptr<Shape> shape, Point p, Vector eye, Vector normal, bool IsInShadow)
{
    Color ambient, diffuse, specular;

	Material m = shape->GetMaterial();

    //Combine the colors of the light and the material
    Color effectiveColor = m.pattern->ColorAtShapePoint(shape->PointToObjectSpace(p)) * intensity;

    //Ambient component
    ambient = effectiveColor * m.ambient;

    //Direction to the light source
    Vector lightVector = (position - p).Normalize();

    //Cosine of the angle between the light and normal vectors
	double lightDotNormal = Vector::DotProduct(normal, lightVector);

    //Negative value means the light source is on the other side of the surface
    if (lightDotNormal < 0.0 || IsInShadow) {
        diffuse = Color(0.0, 0.0, 0.0);
        specular = Color(0.0, 0.0, 0.0);
    }
    else {
        //Contribution of the diffuse component
        diffuse = effectiveColor * m.diffuse * lightDotNormal;

        //Cosine of angle between reflection and eye vectors
		double reflectDotEye = Vector::DotProduct((-lightVector).Reflect(normal), eye);

        if (reflectDotEye <= 0.0) {
            specular = Color(0.0, 0.0, 0.0);
        }
        else {
            //Specular component
            double factor = pow(reflectDotEye, m.shininess);
            specular = intensity * static_cast<double>(m.specular * factor);
        }
    }

    //Add all components to get the final shading
    return ambient + diffuse + specular;
}
