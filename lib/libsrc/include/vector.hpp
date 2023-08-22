#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <iostream>

// === TEMPLATE ===
template<typename T, int N>
class Vector {
// --- Data ---
private:
    std::array<T,N> data;

// --- Constructor and Initialization ---
public:
    Vector() {
        data.fill(Zero<T>::value);
    }

    Vector(T value) {
        data.fill(value);
    }

    Vector(std::array<T,N> values_in)
        :data{ values_in } {}

// --- Gets and Sets ---
public:
    T operator[](int i) {
        return data[i];
    } 

    void set(T value, int i) {
        data[i] = value;
    }

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
        T output = Zero<T>::value;
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
        Vector<T,N> output;
        output.set(One<T>::value, i);
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

// --- Statics ---
private:
    template<typename U> struct Zero {};
    template<> struct Zero<int>   { static constexpr int   value = 0; };
    template<> struct Zero<float> { static constexpr float value = 0.0f; };

    template<typename U> struct One {};
    template<> struct One<int>   { static constexpr int   value = 1; };
    template<> struct One<float> { static constexpr float value = 1.0f; };
};

typedef Vector<float,4> vec4;
typedef Vector<float,3> vec3;
typedef Vector<float,2> vec2;

typedef Vector<int,4> vec4i;
typedef Vector<int,3> vec3i;
typedef Vector<int,2> vec2i;

#endif