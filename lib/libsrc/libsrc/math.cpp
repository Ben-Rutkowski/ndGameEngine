#include "math.hpp"

float math::rads(float value) {
    const float convert = M_PI/180.0f;
    return value*convert;
}