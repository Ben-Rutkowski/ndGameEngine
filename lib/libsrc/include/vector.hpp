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

#endif