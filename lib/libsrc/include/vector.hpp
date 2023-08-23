#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <iostream>

namespace cTemp {
template<typename U> struct Zero {};
template<> struct Zero<int>   { static constexpr int   value = 0; };
template<> struct Zero<float> { static constexpr float value = 0.0f; };

template<typename U> struct One {};
template<> struct One<int>   { static constexpr int   value = 1; };
template<> struct One<float> { static constexpr float value = 1.0f; };
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

-------- MATH --------
- operator+ : Sums two vectors of the same length together.
    ==Parameter==
        - Vector<T,N>& other
    ==Return==
        - Vector<T,N>

- dot : Returns the dot product with a vector of the same length.
    ==Parameter==
        - Vector<T,N>&
    ==Return==
        - T

- normInf : Returns the infinity norm of the vector
    ==Return==
        - T

- norm2 : Returns the L2 norm of the vector
    ==Return==
        - float

-------- STATIC --------
- basis : Returns a basis vector with zeros
    everywhere except at the given index
    ==Parameter==
        - int i
    ==Return==
        Vector<T,N>

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

// --- Math ---
public:
    Vector<T,N> operator+(Vector<T,N>& other) {
        std::array<T,N> output;
        for (int i=0; i<N; i++) {
            output[i] = data[i] + other[i];
        }
        return Vector<T,N>(output);
    }

    T dot(Vector<T,N>& other) {
        T output = cTemp::Zero<T>::value;
        for (int i=0; i<N; i++) {
            output += data[i]*other[i];
        }
        return output;
    }

    T     normInf() { return this->dot(*this); }
    float norm2()   { return sqrt(this->normInf()); }

// --- Static ---
public:
    static Vector<T,N> basis(int i) {
        Vector<T,N> output(cTemp::Zero<T>::value);
        output.set(cTemp::One<T>::value, i);
        return output;
    }

// --- Debugging ---
public:
    void print() {
        for (int i=0; i<N-1; i++) {
            std::cout << data[i] << ", ";
        }
        std::cout << data[N-1] << std::endl;
    }
};

typedef Vector<float,4> vec4;
typedef Vector<float,3> vec3;
typedef Vector<float,2> vec2;

typedef Vector<int,4> vec4i;
typedef Vector<int,3> vec3i;
typedef Vector<int,2> vec2i;

#endif