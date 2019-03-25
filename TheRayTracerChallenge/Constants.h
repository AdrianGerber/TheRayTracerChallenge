#pragma once

#include <cmath>

namespace Constants {
    const float EPSILON = 0.00001f;

    inline static bool FloatEqual(float value1, float value2) {
        return fabsf(value1 - value2) < Constants::EPSILON;
    }
}
