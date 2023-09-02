#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <iostream>

#include "math.hpp"

namespace cTemp {
template<typename U> struct Zero {};
template<> struct Zero<int>    { static constexpr int    value = 0; };
template<> struct Zero<float>  { static constexpr float  value = 0.0f; };
template<> struct Zero<double> { static constexpr double value = 0.0; };

template<typename U> struct One {};
template<> struct One<int>    { static constexpr int    value = 1; };
template<> struct One<float>  { static constexpr float  value = 1.0f; };
template<> struct One<double> { static constexpr double value = 1.0; };
}

/* CLASS TEMPLATE: Vector
    T : type of elements (must have +, * and sqrt defined)
    N : number of elements

======== ATTRIBUTES ========
- array<T,N> data


======== METHODS ========
-------- CONSTRUCTORS --------
- Vector : Creates a vector filled with zeros.

- Vector : Creates a vector filled with given value.
    ==Parameter==
        - T value : The value to fill the vector.

- Vector : Creates a vector with a given list of values
    ==Parameter==
        - array<T,N>& 
            or         : The list to be initialized
        - array<T,N>&&

-------- GETS AND SETS --------
- operator[] : Returns the value at the give index
    ==Parameter==
        - int i
    ==Return==
        - T

- set : Assigned the given value at the given index
    ==Parameters==
        - T value
        - int i

-------- OPERATORS --------
- scalarK : Multiplies the first k elements by a scalar.
- scalar : Multiplies vector by a scalar.

- normalizeK : Normalizes the first k elements of the vector.
- normalize : Normalizes vector

-------- MATH --------
- operator+ : Sums two vectors of the same length together.
    ==Parameter==
        - Vector<T,N>& other
    ==Return==
        - Vector<T,N>

- operator- : Subtracts this vector by other vector (this - other);
    ==Parameter==
        - Vector<T,N>& other
    ==Return==
        - Vector<T,N>

- dotK : Returns the dot product with a vector of the same length considering 
    only the first k elements.
    ==Parameter==
        - Vector<T,N>& other
        - int k 
    ==Return==
        - T

- dot : Returns the dot product with a vector of the same length.
    ==Parameter==
        - Vector<T,N>& other
    ==Return==
        - T

- cross : Returns the cross product of this with another vector.
    Allows N = 3,4 only.
    ==Parameters==
        - Vector<T,3>& or Vector<T,4>&
    ==Returns==
        - Vector<T,3> or Vector<T,4>&

- normInfK : Returns the infinity norm of the first k elements of the vector.
    ==Parameter==
        -int k
    ==Return==
        - T

- normInf : Returns the infinity norm of the vector
    ==Return==
        - T

- norm2K : Returns the L2 norm of the first k elements of the vector.
    ==Parameter==
        -int k
    ==Return==
        - T

- norm2 : Returns the L2 norm of the vector.
    ==Return==
        - float

-------- STATIC --------
- basis : Returns a basis vector with zeros
    everywhere except at the given index
    ==Parameter==
        - int i
    ==Return==
        Vector<T,N>

- tipToTail : Takes two vec4 and returns the vector that goes
    from tip to tail in three dimensions, the fourth component
    is set to One.
        ==Parameters==
            - Vector<T,4>& tip
            - Vector<T,4>& tail
        ==Return==
            - Vector<T,4>

-------- DEBUGGING --------
- print : Prints the vector in a line.

*/

// === TEMPLATE ===
template<typename T, int N>
class Vector {
// --- Attributes ---
private:
    std::array<T,N> data;

// --- Constructors ---
public:
    Vector()        { data.fill(cTemp::Zero<T>::value); }
    Vector(T value) { data.fill(value); }
    Vector(std::array<T,N>& values_in)  :data{ values_in } {}
    Vector(std::array<T,N>&& values_in) :data{ values_in } {}

// --- Gets and Sets ---
public:
    T    operator[](int i)   { return data[i]; } 
    void set(T value, int i) { data[i] = value; }
    bool isFinite() {
        for (int i=0; i<N; i++) {
            if (!isfinite(data[i])) {
                return false;
            }
        } 
        return true;
    }

// --- Operations ---
public:
    void scalar(T c) { scalarK(c, N); }
    void scalarK(T c, int k) {
        for (int i=0; i<k; i++) {
            data[i] = c*data[i];
        }
    }

    void normalize() { normalizeK(N); }
    void normalizeK(int k) {
        T norm = this->norm2K(k);
        scalarK(1/norm, k);
    }
    
    void roundPlace(int k) {
        for (int i=0; i<N; i++) {
            data[i] = math::roundPlace(data[i], k);
        }
    }

// --- Math ---
public:
    Vector<T,N> multK(T scalar, int k) {
        Vector<T,N> output = *this;
        for (int i=0; i<k; i++) {
            output.set(scalar*output[i], i);
        }
        return output;
    }

    Vector<T,N> operator+(Vector<T,N>& other)  { return Vector<T,N>::sumK(*this, other, N); }
    Vector<T,N> operator+(Vector<T,N>&& other) { return Vector<T,N>::sumK(*this, other, N); }
    Vector<T,N> operator-(Vector<T,N>& other)  { return Vector<T,N>::subtrK(*this, other, N); }
    Vector<T,N> operator-(Vector<T,N>&& other) { return Vector<T,N>::subtrK(*this, other, N); }
    T dot(Vector<T,N>& other)  { return dotK(*this, other, N); }
    T dot(Vector<T,N>&& other) { return dotK(*this, other, N); }

    Vector<T,N> cross(Vector<T,N>& b) {
        std::array<T,N> output;
        for (int i=0; i<3; i++) {
            output[i] = crossValue(*this, b, i);
        }
        for (int i=3; i<N; i++) {
            output[i] = cTemp::One<T>::value;
        }
        return Vector<T,N>(output);
    }

    T normInfK(int k) { return dotK(*this, *this, k); }
    T norm2K(int k)   { return sqrt(this->normInfK(k)); }
    T normInf()  { return this->dot(*this); }
    T norm2()    { return sqrt(this->normInf()); }

// --- Static ---
public:
    static Vector<T,N> sumK(Vector<T,N>& a, Vector<T,N>& b, int k) {
        Vector<T,N> output = a;
        for (int i=0; i<k; i++) {
            output.set(a[i] + b[i], i);
        }
        return output;
    }

    static Vector<T,N> subtrK(Vector<T,N>& a, Vector<T,N>& b, int k) {
        Vector<T,N> output = a;
        for (int i=0; i<k; i++) {
            output.set(a[i] - b[i], i);
        }
        return output;
    }

    static T dotK(Vector<T,N>& a, Vector<T,N>& b, int k) {
        T output = a[0]*b[0];
        for (int i=1; i<k; i++) {
            output += a[i]*b[i];
        }
        return output;
    }

    static Vector<T,N> basis(int i) {
        Vector<T,N> output(cTemp::Zero<T>::value);
        output.set(cTemp::One<T>::value, i);
        return output;
    }

    static T crossValue(Vector<T,N>& a, Vector<T,N>& b, int i) {
        return a[(i+1)%3]*b[(i+2)%3] - a[(i+2)%3]*b[(i+1)%3];
    }

    // vector must be normalized
    static Vector<float,4> rightVec(Vector<float,4> front) {
        Vector<float,4> up = Vector<float,4>::basis(1);
        Vector<float,4> right = front.cross(up);
        right.normalizeK(3);
        return right;
    }

    static Vector<float,4> aveUnits4f(Vector<float,4> a, Vector<float,4> b) {
        Vector<float,4> output = sumK(a, b, 3);
        output.scalarK(0.5f, 3);
        return output;
    }

    static Vector<T,N> weightK(T a, Vector<T,N> v1, T b, Vector<T,N> v2, int k) {
        v1.scalarK(a, k);
        v2.scalarK(b, k);
        return sumK(v1, v2, 3);
    }

// --- Debugging ---
public:
    void print() {
        for (int i=0; i<N-1; i++) {
            std::cout << data[i] << ", ";
        }
        std::cout << data[N-1] << std::endl << std::endl;
    }
};

typedef Vector<float,4> vec4;
typedef Vector<float,3> vec3;
typedef Vector<float,2> vec2;

typedef Vector<int,4> vec4i;
typedef Vector<int,3> vec3i;
typedef Vector<int,2> vec2i;

#endif