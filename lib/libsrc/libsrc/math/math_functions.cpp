#include "math/math_functions.hpp"

float math::rads(float value) {
    const float convert = M_PI/180.0f;
    return value*convert;
}

float math::remander(float numerator, float denomenator) {
    float divide = numerator / denomenator;
    float remainder = abs(divide) - floor(abs(divide));

    if (numerator > 0.0f) { return remainder; } 
    else                  { return -remainder; }
}