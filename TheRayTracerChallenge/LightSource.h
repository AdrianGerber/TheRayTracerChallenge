#pragma once

#include "Color.h"
#include "Tuple.h"
#include "Material.h"
#include <cmath>
#include "Pattern.h"
#include "Shape.h"

//Point light
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

	//Calculate the color of a point when illuminated by this light source
    Color Lighting(std::shared_ptr<Shape> shape, Point p, Vector eye, Vector normal, bool IsInShadow);

private:
    Color intensity;
    Point position;
};

inline bool operator==(LightSource l1, LightSource l2) {
    return
        l1.GetIntensity() == l2.GetIntensity()
        && l1.GetPosition() == l2.GetPosition();
}