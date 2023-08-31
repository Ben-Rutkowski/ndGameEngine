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
    Matrix(T uniform) { data.fill(uniform); }
    Matrix(std::array<T,M*N>&& data_in) :data{ data_in } {}
    Matrix(std::array<T,M*N>& data_in)  :data{ data_in } {}

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
    T    get(int i, int j)          { return data[index(i,j)]; }
    void set(T value, int i, int j) { data[index(i,j)] = value; }
    T*   location() { return &data[0]; }

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

    Vector<T,M> operator*(Vector<T,N> vec) {
        T               elem;
        std::array<T,M> output;
        for (int i=0; i<M; i++) {
            elem = get(i,0)*vec[0];
            for (int j=1; j<N; j++) {
                elem = elem + get(i,j)*vec[j];
            }
            output[i] = elem;
        }
        return Vector<T,M>(output);
    }

    template<int P>
    Matrix<T,M,P> operator*(Matrix<T,N,P>& other) {
        T                 elem;
        std::array<T,M*P> output;
        for (int i=0; i<M; i++) {
            for (int k=0; k<P; k++) {
                elem = get(i,0)*other.get(0,k);
                for (int j=1; j<N; j++) {
                    elem = elem + get(i,j)*other.get(j,k);
                }
                output[index(i,k,P)] = elem;
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
                elem = math::roundPlace(get(i,j),place);
                data[index(i,j)] = elem;
            }
        }
    }

// --- Private ---
private:
    static int index(int i, int j)           { return j + N*i; }
    static int index(int i, int j, int COLS) { return j + COLS*i; }

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

    static Matrix<float,4,4> translate(vec4 pos) {
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        for (int i=0; i<3; i++) {
            output.set(pos[i],i,3);
        }
        return output;
    }

    static Matrix<float,4,4> itranslate(vec4 pos) {
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        for (int i=0; i<3; i++) {
            output.set(-pos[i],i,3);
        }
        return output;
    }

    static Matrix<float,4,4> rotX(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        output.set(c,1,1); output.set(s,1,2);
        output.set(-s,2,1); output.set(c,2,2);
        return output;
    }

    static Matrix<float,4,4> rotY(float theta) {
        float c = cos(theta);
        float s = sin(theta);
        Matrix<float,4,4> output = Matrix<float,4,4>::iden();
        output.set(c,0,0); output.set(-s,0,2);
        output.set(s,2,0); output.set(c,2,2);
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

    // Both must be unit length and orthoganal
    static Matrix<float,4,4> lookAt(vec4 front, vec4 right) {
        vec4 up = right.cross(front);
        return Matrix<float,4,4>({
             right[0],  right[1],  right[2], 0.0f,
             up[0],     up[1],     up[2],    0.0f,
            -front[0], -front[1], -front[2], 0.0f,
            0.0f,       0.0f,      0.0f,     1.0f
        });
    }

    static Matrix<float,4,4> view(vec4 pos, vec4 front, vec4 right) {
        Matrix<float,4,4> translate = Matrix<float,4,4>::itranslate(pos);
        Matrix<float,4,4> rot_view = Matrix<float,4,4>::lookAt(front, right);
        return rot_view * translate;
    }

    // fov: the vertical angle, ratio: width over height
    static Matrix<float,4,4> projPer(float fov, float ratio, float near, float far) {
        float n = near;
        float f = far;
        float h = n*tan(fov);
        float w = h*ratio;

        return Matrix<float,4,4>({
            n/w,  0.0f,  0.0f,         0.0f,
            0.0f, n/h,   0.0f,         0.0f,
            0.0f, 0.0f, -(f+n)/(f-n), -2*f*n/(f-n),
            0.0f, 0.0f, -1.0f,         0.0f
        });
    }
};

typedef Matrix<float,4,4> mat4;
typedef Matrix<float,3,3> mat3;
typedef Matrix<float,2,2> mat2;

#endif