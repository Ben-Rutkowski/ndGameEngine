#ifndef MATH_HPP
#define MATH_HPP

#include <math.h>

namespace math {

template<typename T>
T roundPlace(T value, int place) {
    float factor = pow(10,place);
    T integer    = floor(value);
    T mantissa   = value - integer;
    return integer + (floor(mantissa * factor + 0.5)/factor);
}

float rads(float value);
float remander(float numerator, float denomenator);
// coordinate in clip space to the pixel in the span  
// eg 0.0 -> 300 when width is 600
float clipToPixel(float coord, int pixel_span); 


}

#endif