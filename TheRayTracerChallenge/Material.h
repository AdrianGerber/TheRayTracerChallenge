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

    Material();
    ~Material();

    bool operator==(Material m2) {
        return
            pattern == m2.pattern
            && Constants::DoubleEqual(ambient, m2.ambient)
            && Constants::DoubleEqual(diffuse, m2.diffuse)
            && Constants::DoubleEqual(specular, m2.specular)
            && Constants::DoubleEqual(shininess, m2.shininess);
    }

    bool operator!=(Material m2) {
        return !(*this == m2);
    }
};

