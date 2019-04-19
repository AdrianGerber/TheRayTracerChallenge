#pragma once
#include "Color.h"
#include "Constants.h"
#include "Pattern.h"
#include "ColorPattern.h"
#include <memory>

//Describes the properties of a shape's surface / material
class Material
{
public:
    //Pattern applied to the surface
	std::shared_ptr<Pattern> pattern;

	//Lighting properties
	double ambient;
	double diffuse;
	double specular;
	double shininess;

	//Reflection and refraction
	double reflective;
	double transparency;
	double refractiveIndex;

	Material();
    ~Material() = default;

	bool operator==(Material m2);

	bool operator!=(Material m2);

	//Create a material that looks like glass
	static Material CreateGlass();
};


inline Material::Material()
{
	auto color = std::make_shared<ColorPattern>();
	color->SetColor(Color(1.0f, 1.0f, 1.0f));

	pattern = color;

	ambient = 0.1;
	diffuse = 0.9;
	specular = 0.9;
	shininess = 200.0;
	reflective = 0.0;
	transparency = 0.0;
	refractiveIndex = 1.0;
}

inline bool Material::operator==(Material m2) {
	return
		pattern == m2.pattern
		&& Constants::DoubleEqual(ambient, m2.ambient)
		&& Constants::DoubleEqual(diffuse, m2.diffuse)
		&& Constants::DoubleEqual(specular, m2.specular)
		&& Constants::DoubleEqual(reflective, m2.reflective)
		&& Constants::DoubleEqual(shininess, m2.shininess)
		&& Constants::DoubleEqual(transparency, m2.transparency)
		&& Constants::DoubleEqual(refractiveIndex, m2.refractiveIndex);
}

inline bool Material::operator!=(Material m2) {
	return !(*this == m2);
}

inline Material Material::CreateGlass() {
	Material m;
	m.pattern = std::make_shared<ColorPattern>(Color(0.0, 0.0, 0.0));
	m.refractiveIndex = 1.5;
	m.transparency = 1.0;

	return m;
}