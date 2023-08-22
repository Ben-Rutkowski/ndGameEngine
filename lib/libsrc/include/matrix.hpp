/*

Matrices have m rows and n columns. The data is stored as column major.
That is n vectors of m elements.

*/

#include "vector.hpp"

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

// === TEMPLATE ===
template<typename T, int M, int N>
class MatrixTemplate {
// --- Attributes ---
protected:
    std::array<VectorTemplate<T, M>, N> data;

// --- Constructors and Initialization ---
public:
    MatrixTemplate(std::array<VectorTemplate<T, M>, N> data_in)
        :data{ data_in } {}

    T get(int i, int j) {
        return data[j][i];
    }
};

// === CLASSES ===
class mat4f : public MatrixTemplate<float,4,4> {
public:
    mat4f();
    mat4f(vec4 arg0, vec4 arg1, vec4 arg2, vec4 arg3);
};

#endif