#include "Material.h"



Material::Material()
{
	auto color = std::make_shared<ColorPattern>();
	color->SetColor(Color(1.0f, 1.0f, 1.0f));
	
	pattern = color;
	
    ambient = 0.1f;
    diffuse = 0.9f;
    specular = 0.9f;
    shininess = 200.0f;
}


Material::~Material()
{
}
