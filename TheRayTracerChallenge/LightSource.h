#pragma once

#include "Color.h"
#include "Tuple.h"
#include "Material.h"
#include <cmath>

class LightSource
{
public:
    LightSource(Point position, Color intensity);
    LightSource();
    ~LightSource();

    Color GetIntensity() { return intensity; }
    void SetIntensity(Color intensity) { this->intensity = intensity; }

    Point GetPosition() { return position; }
    void SetPosition(Point position) { this->position = position; }

    Color Lighting(Material m, Point p, Vector eye, Vector normal);

private:
    Color intensity;
    Point position;
};

