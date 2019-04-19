#pragma once
#include "Color.h"
#include "Constants.h"
#include "Pattern.h"
#include "ColorPattern.h"
#include <memory>

class Material
{
public:
    std::shared_ptr<Pattern> pattern;
	double ambient;
	double diffuse;
	double specular;
	double shininess;
	double reflective;
	double transparency;
	double refractiveIndex;

    Material();
    ~Material();

    bool operator==(Material m2) {
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

    bool operator!=(Material m2) {
        return !(*this == m2);
    }

	static Material CreateGlass() {
		Material m;
		m.pattern = std::make_shared<ColorPattern>(Color(0.0, 0.0, 0.0));
		m.refractiveIndex = 1.5;
		m.transparency = 1.0;

		return m;
	}
};

