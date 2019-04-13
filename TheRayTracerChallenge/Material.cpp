#include "Material.h"



Material::Material()
{
    color = Color(1.0f, 1.0f, 1.0f);
    ambient = 0.1f;
    diffuse = 0.9f;
    specular = 0.9f;
    shininess = 200.0f;
}


Material::~Material()
{
}
