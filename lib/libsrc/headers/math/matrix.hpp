#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "math/vector.hpp"

// !! Remember matrix stored in column major !!
#pragma pack(push,1)
template<typename T, int M, int N>
class ndMatrix {
private:
    std::array<T,M*N> data;

// --- Constructors ---
public:
    ndMatrix() {}
    ndMatrix(T const value) { data.fill(value); }
    ndMatrix(const std::initializer_list<T>& list) {
        auto value = list.begin();
        for (int i=0; i<M*N; i++) {
            data[i] = *value;
            value++;
        }
    }
    inline int idx(const int i, const int j) const { return i+M*j; }

    T& operator[](const int k) { return data[k]; }
    T  get(const int k) const  { return data[k]; }
    T& at(const int i, const int j)       { return data[idx(i,j)]; }
    T get(const int i, const int j) const { return data[idx(i,j)]; }

// --- Operatoions ---
public:
    void sum(const ndMatrix<T,M,N>& B) {
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                at(i,j) += B.get(i,j);
            }
        }
    }

    void sub(const ndMatrix<T,M,N>& B) {
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                at(i,j) -= B.get(i,j);
            }
        }
    }

    void scalar(const T c) {
        for (int k=0; k<M*N; k++) {
            data[k] *= c;
        }
    }

// --- Math ---
public:
    ndMatrix<T,M,N> operator+(const ndMatrix<T,M,N>& B) const {
        ndMatrix<T,M,N> output;
        for (int k=0; k<M*N; k++) {
            output[k] = get(k) + B.get(k);
        }
        return output;
    }

    ndMatrix<T,M,N> operator-(const ndMatrix<T,M,N>& B) const {
        ndMatrix<T,M,N> output;
        for (int k=0; k<M*N; k++) {
            output[k] = get(k) - B.get(k);
        }
        return output;
    }

    // ndMatrix<T,M,N> operator*(const T c);  --  written as friend function

    ndVector<T,N> operator*(const ndVector<T,N>& x) const {
        ndVector<T,N> output;
        for (int i=0; i<M; i++) {
            output[i] = vConst<T>::zero;
            for (int j=0; j<N; j++) {
                output[i] += get(i,j)*x.get(j);
            }
        }
        return output;
    }

    template<int K>
    ndMatrix<T,M,K> operator*(const ndMatrix<T,N,K>& B) const {
        ndMatrix<T,M,K> output;
        for (int i=0; i<M; i++) {
            for (int k=0; k<K; k++) {
                output.at(i,k) = vConst<T>::zero;
                for (int j=0; j<N; j++) {
                    output.at(i,k) += get(i,j)*B.get(j,k);
                }
            }
        }
        return output;
    }

// --- Static ---
private:
    static const int LEAST = std::min(M,N);

public:
    static ndMatrix<T,M,N> makeTranspose(const std::array<T,M*N>&& row_maj) {
        ndMatrix<T,M,N> output;
        for (int i=0; i<M; i++) {
            for (int j=0; j<N; j++) {
                output.at(i,j) = row_maj[j+N*i];
            }
        }
        return output;
    }

    static ndMatrix<T,M,N> iden() {
        ndMatrix<T,M,N> output(vConst<T>::zero);
        for (int k=0; k<LEAST; k++) {
            output.at(k,k) = vConst<T>::one;
        }
        return output;
    }

// --- Debug ---
public:
    void debug() {
        for (int i=0; i<M; i++) {
            std::cout << "[ ";
            for (int j=0; j<N; j++) {
                std::cout << get(i,j) << " ";
            }
            std::cout << "]\n";
        }
    }

};
#pragma pack(pop)


// ======== Friend Functions ========
template<typename T, int M, int N>
ndMatrix<T,M,N> operator*(const T c, const ndMatrix<T,M,N>& A) {
    ndMatrix<T,M,N> output;
    for (int k=0; k<M*N; k++) {
        output[k] = c*A.get(k);
    }
    return output;
}

typedef ndMatrix<float,2,2> mat2;
typedef ndMatrix<float,3,3> mat3;
typedef ndMatrix<float,4,4> mat4;

#endif
