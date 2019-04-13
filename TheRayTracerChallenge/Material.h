#pragma once
#include "Color.h"
#include "Constants.h"

class Material
{
public:
    Color color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    Material();
    ~Material();

    bool operator==(Material m2) {
        return
            color == m2.color
            && Constants::FloatEqual(ambient, m2.ambient)
            && Constants::FloatEqual(diffuse, m2.diffuse)
            && Constants::FloatEqual(specular, m2.specular)
            && Constants::FloatEqual(shininess, m2.shininess);
    }

    bool operator!=(Material m2) {
        return !(*this == m2);
    }
};

