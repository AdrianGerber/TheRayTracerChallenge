#pragma once
#include "Color.h"
#include "Constants.h"

class Material
{
public:
    Color color;
	double ambient;
	double diffuse;
	double specular;
	double shininess;

    Material();
    ~Material();

    bool operator==(Material m2) {
        return
            color == m2.color
            && Constants::DoubleEqual(ambient, m2.ambient)
            && Constants::DoubleEqual(diffuse, m2.diffuse)
            && Constants::DoubleEqual(specular, m2.specular)
            && Constants::DoubleEqual(shininess, m2.shininess);
    }

    bool operator!=(Material m2) {
        return !(*this == m2);
    }
};

