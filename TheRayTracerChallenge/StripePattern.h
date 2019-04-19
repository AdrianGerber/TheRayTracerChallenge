#pragma once
#include "Pattern.h"
#include <cmath>
#include <memory>
#include "ColorPattern.h"


//Pattern that alternates between two colors / other patterns
class StripePattern : public Pattern
{
private:
	std::shared_ptr<Pattern> pattern1;
	std::shared_ptr<Pattern> pattern2;

public:
	StripePattern();

	//Construct using two colors
	StripePattern(Color c1, Color c2);

	//Construct with two other patterns
	StripePattern(std::shared_ptr<Pattern> p1, std::shared_ptr<Pattern> p2);

	~StripePattern() = default;


	Color ReadPattern(Point point) override;

	void SetPattern1(std::shared_ptr<Pattern> pattern) { pattern1 = pattern; }
	void SetPattern2(std::shared_ptr<Pattern> pattern) { pattern2 = pattern; }

	std::shared_ptr<Pattern> GetPattern1() { return pattern1; }
	std::shared_ptr<Pattern> GetPattern2() { return pattern2; }
};


inline StripePattern::StripePattern() : StripePattern(Color(0.0, 0.0, 0.0), Color(1.0, 1.0, 1.0)) {

}

//Construct using two colors
inline StripePattern::StripePattern(Color c1, Color c2) {
	//Convert colors to 'color patterns'
	SetPattern1(std::make_shared<ColorPattern>(c1));
	SetPattern2(std::make_shared<ColorPattern>(c2));
}

//Construct with two other patterns
inline StripePattern::StripePattern(std::shared_ptr<Pattern> p1, std::shared_ptr<Pattern> p2) {
	SetPattern1(p1);
	SetPattern2(p2);
}

inline Color StripePattern::ReadPattern(Point point) {

	if (Constants::DoubleEqual(fmod(floor(point.x), 2.0), 0.0)) {
		return pattern1->ColorAtPoint(point, Transform());
	}
	return pattern2->ColorAtPoint(point, Transform());
}
