#pragma once

#include "Pattern.h"
#include "ColorPattern.h"
#include <cmath>
#include <memory>

class CheckerPattern : public Pattern
{
public:
	
	CheckerPattern() : CheckerPattern(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0)){
	
	}

	CheckerPattern(Color c1, Color c2) {
		pattern1 = std::make_shared<ColorPattern>(c1);
		pattern2 = std::make_shared<ColorPattern>(c2);
	}

	CheckerPattern(std::shared_ptr<Pattern> p1, std::shared_ptr<Pattern> p2) {
		pattern1 = p1;
		pattern2 = p2;
	}

	~CheckerPattern() = default;

	Color ReadPattern(Point point) override {
		if (Constants::DoubleEqual(fmod(floor(point.x) + floor(point.y) + floor(point.z), 2.0), 0.0)) {
			return pattern1->ColorAtPoint(point, Transform());
		}

		return pattern2->ColorAtPoint(point, Transform());
	}
	

	void SetPattern1(std::shared_ptr<Pattern> p1) { pattern1 = p1; }
	void SetPattern2(std::shared_ptr<Pattern> p2) { pattern2 = p2; }

	std::shared_ptr<Pattern> GetPattern1() { return pattern1; }
	std::shared_ptr<Pattern> GetPattern2() { return pattern2; }


private:
	std::shared_ptr<Pattern> pattern1, pattern2;
};

