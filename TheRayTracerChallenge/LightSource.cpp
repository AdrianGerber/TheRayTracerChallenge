#include "LightSource.h"



LightSource::LightSource(Point position, Color intensity) 
{
    SetIntensity(intensity);
    SetPosition(position);
}

LightSource::LightSource()
{
    SetIntensity(Color(1.0f, 1.0f, 1.0f));
    SetPosition(Point::CreatePoint(0.0f, 0.0f, 0.0f));
}


LightSource::~LightSource()
{
}

Color LightSource::Lighting(Material m, Point p, Vector eye, Vector normal)
{
    Color ambient, diffuse, specular;

    //Combine the colors of the light and the material
    Color effectiveColor = m.color * intensity;

    //Ambient component
    ambient = effectiveColor * m.ambient;

    //Direction to the light source
    Vector lightVector = (position - p).Normalize();

    //Cosine of the angle between the light and normal vectors
    float lightDotNormal = Vector::DotProduct(normal, lightVector);

    //Negative value means the light source is on the other side of the surface
    if (lightDotNormal < 0.0f) {
        diffuse = Color(0.0f, 0.0f, 0.0f);
        specular = Color(0.0f, 0.0f, 0.0f);
    }
    else {
        //Contribution of the diffuse component
        diffuse = effectiveColor * m.diffuse * lightDotNormal;

        //Cosine of angle between reflection and eye vectors
        float reflectDotEye = Vector::DotProduct((-lightVector).Reflect(normal), eye);

        if (reflectDotEye <= 0.0f) {
            specular = Color(0.0f, 0.0f, 0.0f);
        }
        else {
            //Specular component
            volatile float factor = powf(reflectDotEye, m.shininess);
            specular = intensity * m.specular * factor;
        }
    }

    //Add all components to get the final shading
    return ambient + diffuse + specular;
}
