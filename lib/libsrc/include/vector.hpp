#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>

// === TEMPLATE ===
template<typename T, int N>
class VectorTemplate {
// --- Data ---
protected:
    std::array<T,N> data;

// --- Constructor and Initialization ---
public:
    VectorTemplate(std::array<T,N> input)
        :data{ input } {}
    
    T operator[](int index) {
        return data[index];
    }
};

// === CLASSES ===
class vec2i : public VectorTemplate<int,2> {
public:
    vec2i();
    vec2i(int arg0, int arg1);
};

class vec3 : public VectorTemplate<float, 3> {
public:
    vec3();
    vec3(float arg0, float arg1, float arg2);
};

class vec2 : public VectorTemplate<float, 2> {
public:
    vec2();
    vec2(float arg0, float arg1);
};

#endif