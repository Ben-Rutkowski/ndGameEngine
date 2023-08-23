/* CLASS TEMPLATE: Matrix
    T : type of the elements (must have + and * defined)
    M : number of rows
    N : number of columns

Matrices have m rows and n columns. The data is stored as row major.

*/

#include "vector.hpp"

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
        constexpr int small = std::min(M,N);

        Matrix<T,M,N> output(cTemp::Zero<T>::value);
        for (int i=0; i<small; i++) {
            output.set(cTemp::One<T>::value, i, i);
        }

        return output;
    }
};

typedef Matrix<float,4,4> mat4;
typedef Matrix<float,3,3> mat3;
typedef Matrix<float,2,2> mat2;

#endif