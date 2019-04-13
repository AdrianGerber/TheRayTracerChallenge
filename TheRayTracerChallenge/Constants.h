#pragma once

#include <cmath>

namespace Constants {
    const float EPSILON = 0.000018f;
    const float PI = 3.14159265358979323846f;

    inline bool FloatEqual(float value1, float value2) {
        return fabsf(value1 - value2) < Constants::EPSILON;
    }
}
