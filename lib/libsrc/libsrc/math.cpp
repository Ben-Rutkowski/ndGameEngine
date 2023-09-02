#include "math.hpp"

float math::rads(float value) {
    const float convert = M_PI/180.0f;
    return value*convert;
}

float math::remander(float numerator, float denomenator) {
    float divide = numerator / denomenator;
    return divide - floor(divide);
}