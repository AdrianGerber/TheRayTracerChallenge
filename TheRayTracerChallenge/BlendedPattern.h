#pragma once
#pragma once

#include "Pattern.h"
#include "ColorPattern.h"
#include <cmath>
#include <memory>

class BlendedPattern : public Pattern
{
public:

	BlendedPattern() {
		pattern1 = pattern2 = std::make_shared<ColorPattern>(Color(1.0, 1.0, 1.0));
	}


	BlendedPattern(std::shared_ptr<Pattern> p1, std::shared_ptr<Pattern> p2) {
		pattern1 = p1;
		pattern2 = p2;
	}

	~BlendedPattern() = default;

	Color ReadPattern(Point point) override {
		//Blend both patterns together
		return pattern1->ColorAtPoint(point, Transform()) + pattern2->ColorAtPoint(point, Transform());
	}


	void SetPattern1(std::shared_ptr<Pattern> p1) { pattern1 = p1; }
	void SetPattern2(std::shared_ptr<Pattern> p2) { pattern2 = p2; }

	std::shared_ptr<Pattern> GetPattern1() { return pattern1; }
	std::shared_ptr<Pattern> GetPattern2() { return pattern2; }


private:
	std::shared_ptr<Pattern> pattern1, pattern2;
};

