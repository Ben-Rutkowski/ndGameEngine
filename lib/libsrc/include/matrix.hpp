/* CLASS TEMPLATE: Matrix
    T : type of the elements (must have + and * defined)
    M : number of rows
    N : number of columns

Matrices have m rows and n columns. The data is stored as row major.

*/

#include "vector.hpp"
#include "math.hpp"

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <algorithm>

// === TEMPLATE ===
template<typename T, int M, int N>
class Matrix {
// --- Attributes ---
private:
    std::array<T,M*N> data;
    
    static const int LEAST = std::min(M,N);

// --- Constructors and Initialization ---
public:
    Matrix(T uniform) {
        data.fill(uniform);
    }

    Matrix(std::array<T,M*N>&& data_in)
        :data{ data_in } {}

    Matrix(std::array<T,M*N>& data_in)
        :data{ data_in } {}

    Matrix(std::array<Vector<T,M>,N>& columns) {
        for(int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                data[index(i,j)] = columns[j][i];
            }
        }
    }

    Matrix(std::array<Vector<T,M>,N>&& columns) {
        for(int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                data[index(i,j)] = columns[j][i];
            }
        }
    }

// --- Gets and Sets ---
public:
    T get(int i, int j) {
        return data[index(i,j)];
    }

    void set(T value, int i, int j) {
        data[index(i,j)] = value;
    }

// --- Math ---
public:
    Matrix<T,M,N> operator+(Matrix<T,M,N>& other) {
        std::array<T,M*N> output;
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                output[index(i,j)] = get(i,j) + other.get(i,j);
            }
        }

        return Matrix<T,M,N>(output);
    }

    template<int P>
    Matrix<T,M,P> operator*(Matrix<T,N,P>& other) {
        std::array<T,M*P> output;

        T element;
        for (int i=0; i<M; i++) {
            for (int k=0; k<P; k++) {
                element = get(i,0)*other.get(0,k);
                for (int j=1; j<N; j++) {
                    element = element + get(i,j)*other.get(j,k);
                }
                output[index(i,k,P)] = element;
            }
        }
        return Matrix<T,M,P>(output);
    }

// --- Operations ---
public:
    Matrix<T,N,M> transpose() {
        std::array<T,M*N> output;
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                output[index(i,j)] = get(j,i);
            }
        }

        return Matrix<T,N,M>(output);
    }

    void roundPlace(int place) {
        T elem;
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                elem = math::roundPlace(data[index(i,j)],place);
                data[index(i,j)] = elem;
            }
        }
    }

// --- Private ---
private:
    int index(int i, int j)           { return j + N*i; }
    int index(int i, int j, int COLS) { return j + COLS*i; }

// --- Debugging ---
public:
    void print() {
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                std::cout << get(i,j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

// --- Static ---
public:
    static Matrix<T,M,N> iden() {
        Matrix<T,M,N> output(cTemp::Zero<T>::value);
        for (int i=0; i<LEAST; i++) {
            output.set(cTemp::One<T>::value, i, i);
        }
        return output;
    }

    static Matrix<T,M,N> diag(Vector<T,LEAST> vector) {
        Matrix<T,M,N> output(cTemp::Zero<T>::value);
        for (int i=0; i<LEAST; i++) {
            output.set(vector[i], i, i);
        }
        return output;
    }

    static Matrix<float,4,4> scale(vec3 scale) {
        vec4 diagonal({scale[0], scale[1], scale[2], 1.0f});
        return diag(diagonal);
    }

    static Matrix<float,4,4> scale(float scale) {
        vec4 diagonal({scale, scale, scale, 1.0f});
        return diag(diagonal);
    }

    static Matrix<float,4,4> translate(vec3 pos) {
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        for (int i=0; i<3; i++) {
            output.set(pos[i],i,3);
        }
        return output;
    }

    static Matrix<float,4,4> rotX(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        output.set(c,1,1); output.set(-s,1,2);
        output.set(s,2,1); output.set(c,2,2);
        return output;
    }

    static Matrix<float,4,4> rotY(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        output.set(c,0,0); output.set(s,0,2);
        output.set(-s,2,0); output.set(c,2,2);
        return output;
    }

    static Matrix<float,4,4> rotZ(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        output.set(c,0,0); output.set(-s,0,1);
        output.set(s,1,0); output.set(c,1,1);
        return output;
    }
};

typedef Matrix<float,4,4> mat4;
typedef Matrix<float,3,3> mat3;
typedef Matrix<float,2,2> mat2;

#endif