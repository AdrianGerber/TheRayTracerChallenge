#pragma once

#include "Pattern.h"
#include <cmath>

class RingPattern : public Pattern
{
public:
	
	RingPattern() {
		color1 = Color(0.0, 0.0, 0.0);
		color2 = Color(1.0, 1.0, 1.0);
	}

	RingPattern(Color c1, Color c2) { color1 = c1; color2 = c2; }
	~RingPattern() = default;

	Color ReadPattern(Point point) override {
		double distance = sqrt((point.x * point.x) + (point.z * point.z));
		if (Constants::DoubleEqual(fmod(floor(distance), 2.0), 0.0)) {
			return color1;
		}

		return color2;
	}

	void SetColor1(Color c1) { color1 = c1; }
	void SetColor2(Color c2) { color2 = c2; }

	Color GetColor1() { return color1; }
	Color GetColor2() { return color2; }


private:
	Color color1, color2;
};

