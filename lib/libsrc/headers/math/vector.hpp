#ifndef VECTOR_HPP
#define VECTOR_HPP
#define __ND_VECTOR__

#include <array>
#include <iostream>

// ==== Constants ====
template<typename T> struct vConst {};
template<> struct vConst<int> { 
    static constexpr int zero = 0; 
    static constexpr int one  = 1; 
};

template<> struct vConst<float> { 
    static constexpr int zero = 0.0f; 
    static constexpr int one  = 1.0f; 
};
// ==== Constants ====

#pragma pack(push,1)
template<typename T, int N>
class ndVector {
private:
    std::array<T,N> data;

// --- Constructors ---
public:
    ndVector() {}
    ndVector(T value) { data.fill(value); }
    ndVector(const std::initializer_list<T>& list) {
        auto value = list.begin();
        for (int i=0; i<N; i++) {
            data[i] = *value;
            value++;
        }
    }

    T& operator[](const int i) { return data[i]; }

    T get(const int i) const {
        return data[i];
    }

// --- Operations ---
public:
    void sum(const ndVector<T,N>& b) {
        for (int i=0; i<N; i++) {
            data[i] += b.get(i);
        }
    }

    void sub(const ndVector<T,N>& b) {
        for (int i=0; i<N; i++) {
            data[i] -= b.get(i);
        }
    }

    void scalar(const T c) { 
        for (int i=0; i<N; i++) {
            data[i] *= c;
        }
    }

    void normalizeL2() {
        T norm = this->normL2();
        scalar(1.0/norm);
    }

// --- Math ---
public:
    ndVector<T,N> operator+(const ndVector<T,N>& b) {
        ndVector<T,N> output;
        for (int i=0; i<N; i++) {
            output[i] = data[i] + b.get(i);
        }
        return output;
    }

    ndVector<T,N> operator-(const ndVector<T,N>& b) {
        ndVector<T,N> output;
        for (int i=0; i<N; i++) {
            output[i] = data[i] - b.get(i);
        }
        return output;
    }

    // ndVector<T,N> operator*(const T c);  --  written as friend function

    T dot(const ndVector<T,N>& b) {
        T dot = vConst<T>::zero;
        for (int i=0; i<N; i++) {
            dot += data[i]*b.get(i);
        }
        return dot;
    }

    T normL2() {
        T norm = this->dot(*this);
        return sqrt(norm);
    }

// --- Static ---
public:
    static ndVector<T,N> basis(const int i) {
        ndVector<T,N> output(vConst<T>::zero);
        output[i] = vConst<T>::one;
        return output;
    }

// --- Debug ---
public:
    void debug() {
        std::cout << "[ ";
        for (int i=0; i<N; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << "]\n";
    }

};
#pragma pack(pop)


// ======== Friend Functions ========
template<typename T, int N>
ndVector<T,N> operator*(const T c, const ndVector<T,N>& v) {
    ndVector<T,N> output;
    for (int i=0; i<N; i++) {
        output[i] = c*v.get(i);
    }
    return output;
}

typedef ndVector<float,2> vec2;
typedef ndVector<float,3> vec3;
typedef ndVector<float,4> vec4;

typedef ndVector<int,2> vec2i;
typedef ndVector<int,3> vec3i;
typedef ndVector<int,4> vec4i;

#endif
