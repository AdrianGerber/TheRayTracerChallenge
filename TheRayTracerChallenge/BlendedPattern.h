#pragma once
#pragma once

#include "Pattern.h"
#include "ColorPattern.h"
#include <cmath>
#include <memory>


//Pattern that combines two other patterns by addition
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
		return pattern1->ColorAtShapePoint(point) + pattern2->ColorAtShapePoint(point);
	}


	void SetPattern1(std::shared_ptr<Pattern> p1) { pattern1 = p1; }
	void SetPattern2(std::shared_ptr<Pattern> p2) { pattern2 = p2; }

	std::shared_ptr<Pattern> GetPattern1() { return pattern1; }
	std::shared_ptr<Pattern> GetPattern2() { return pattern2; }

	

private:
	std::shared_ptr<Pattern> pattern1, pattern2;

	std::shared_ptr<Pattern> PatternSpecificCopy() override {
		auto p1Copy = pattern1->Copy();
		auto p2Copy = pattern2->Copy();

		return std::make_shared<BlendedPattern>(p1Copy, p2Copy);
	}
};

