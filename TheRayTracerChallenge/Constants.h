#pragma once

#include <cmath>

namespace RayTracer {
	namespace Constants {
		//If the distance between two floating point numbers is smaller than
		//EPSILON, they are treated as equal
		const double EPSILON = 0.00001;

		const double PI = 3.14159265358979323846;

		//Compare two floating point numbers
		inline bool DoubleEqual(double value1, double value2) {
			return fabs(value1 - value2) < Constants::EPSILON;
		}
	}
}