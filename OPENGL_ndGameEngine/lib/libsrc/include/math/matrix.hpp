/* CLASS TEMPLATE: Matrix
    T : type of the elements (must have + and * defined)
    M : number of rows
    N : number of columns

Matrices have m rows and n columns. The data is stored as row major.

*/

#include "math/vector.hpp"
#include "math/math_functions.hpp"

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include <algorithm>

// === TEMPLATE ===
template<typename T, int M, int N>
class Matrix {
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

    T det3() {
        T term_0 = get(0,0)*det2_3x3(0);
        T term_1 = get(1,0)*det2_3x3(1);
        T term_2 = get(2,0)*det2_3x3(2);
        return term_0 + term_1 + term_2;
    }

    T det2_3x3(int k) {
        int i = (k+1)%3;
        int j = (k+2)%3;
        return get(i,1)*get(j,2) - get(i,2)*get(j,1);
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

    static Matrix<T,M,N> replaceColumn(Matrix<T,M,N> mat, Vector<T,M> v, int k) {
        for (int i=0; i<M; i++) {
            mat.set(v[i], i, k);
        }
        return mat;
    }

    static Vector<T,3> cramer3(Matrix<T,M,N> A, Vector<T,M> b) {
        T denom = 1.0f/A.det3();
        T x_num = replaceColumn(A,b,0).det3();
        T y_num = replaceColumn(A,b,1).det3();
        T z_num = replaceColumn(A,b,2).det3();
        return Vector<T,3>({x_num*denom, y_num*denom, z_num*denom});
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
        float w = ratio*h;

        return Matrix<float,4,4>({
            n/w,  0.0f,  0.0f,         0.0f,
            0.0f, n/h,   0.0f,         0.0f,
            0.0f, 0.0f, -(f+n)/(f-n), -2*f*n/(f-n),
            0.0f, 0.0f, -1.0f,         0.0f
        });
    }

    static Matrix<float,4,4> pixelToClip(float width, float height) {
        return Matrix<float,4,4>({
            2.0f/width, 0.0f,        0.0f, -1.0f,
            0.0f,       2.0f/height, 0.0f, -1.0f,
            0.0f,       0.0f       , 0.0f,  0.0f,
            0.0f,       0.0f       , 0.0f,  1.0f,
        });
    }

    static Matrix<float,4,4> selectProjPartial(vec2 br, vec2 tl, float w, float h, float n) {
        float ixd = 1.0f/(tl[0] - br[0]);
        float iyd = 1.0f/(tl[1] - br[1]);
        float iw  = 1.0f/w;
        float ih  = 1.0f/h;
        float in  = 1.0f/n;

        return Matrix<float,4,4>({
            ixd*iw, 0.0f,    br[0]*ixd*in, 0.0f,
            0.0f,   iyd*ih,  br[1]*iyd*in, 0.0f,
            0.0f,   0.0f,   -in,           0.0f,
            0.0f,   0.0f,    0.0f,         1.0f
        });
    }
};

typedef Matrix<float,4,4> mat4;
typedef Matrix<float,3,3> mat3;
typedef Matrix<float,2,2> mat2;

#endif