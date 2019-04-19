#include "Material.h"



Material::Material()
{
	auto color = std::make_shared<ColorPattern>();
	color->SetColor(Color(1.0f, 1.0f, 1.0f));
	
	pattern = color;
	
    ambient = 0.1;
    diffuse = 0.9;
    specular = 0.9;
    shininess = 200.0;
	reflective = 0.0;
}


Material::~Material()
{
}
