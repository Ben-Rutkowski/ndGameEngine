#ifndef MATH_HPP
#define MATH_HPP

namespace math {

template<typename T>
T roundPlace(T value, int place) {
    float factor = pow(10,place);
    T integer    = floor(value);
    T mantissa   = value - integer;
    return integer + (floor(mantissa * factor + 0.5)/factor);
}

}

#endif