#pragma once

#include "Pattern.h"

class ColorPattern : public Pattern
{
public:
	ColorPattern() = default;
	ColorPattern(Color c) { color = c; }
	~ColorPattern() = default;

	Color ReadPattern(Point point) override {
		return color;
	}


	void SetColor(Color c) { color = c; }

	Color GetColor() { return color; }


private:
	Color color;
};

