#pragma once

#include <cmath>

namespace Constants {
	const double EPSILON = 0.00001;
    const double PI = 3.14159265358979323846;

    inline bool DoubleEqual(double value1, double value2) {
        return fabs(value1 - value2) < Constants::EPSILON;
    }
}
