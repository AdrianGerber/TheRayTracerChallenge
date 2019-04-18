#pragma once

#include "Pattern.h"
#include <cmath>

class GradientPattern : public Pattern
{
public:
	GradientPattern() {
		color1 = Color(0.0, 0.0, 0.0);
		color2 = Color(1.0, 1.0, 1.0);
	}

	GradientPattern(Color c1, Color c2) { color1 = c1; color2 = c2; }
	~GradientPattern() = default;

	Color ReadPattern(Point point) override {
		Color colorDistance = color2 - color1;

		double distanceFraction = point.x - floor(point.x);

		//Linear interpolation
		return color1 + (colorDistance * distanceFraction);
	}


	void SetColor1(Color c1) { color1 = c1; }
	void SetColor2(Color c2) { color2 = c2; }

	Color GetColor1() { return color1; }
	Color GetColor2() { return color2; }


private:
	Color color1, color2;
};

