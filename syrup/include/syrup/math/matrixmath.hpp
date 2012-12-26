#ifndef SYRUP_MATRIX_MATH
#define SYRUP_MATRIX_MATH

#include <string.h>

namespace syrup {
    template<typename T, int M, int N = 1>
    struct Matrix {
        T data[M][N]; // Column major
        Matrix() { memset(data, 0, sizeof(data)); }
        Matrix(const Matrix<T, M, N>& e) {
            memcpy(data, e.data, sizeof(data));
        }
        Matrix<T, M, N>& operator=(const Matrix<T, M, N>& e) {
            memcpy(data, e.data, sizeof(data));
            return *this;
        }
        Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& e) {
            for(int i = 0; i < M; ++i) {
                for(int j = 0; j < N; ++j) {
                    data[i][j] += e.data[i][j];
                }
            }
            return *this;
        }
        Matrix<T, M, N>& operator-=(const Matrix<T, M, N>& e) {
            for(int i = 0; i < M; ++i) {
                for(int j = 0; j < N; ++j) {
                    data[i][j] -= e.data[i][j];
                }
            }
            return *this;
        }
        Matrix<T, M, N>& operator*=(const Matrix<T, N, N>& B) {
            Matrix<T, M, N> A(*this);
            memset(data, 0, sizeof(data));
            for(int i = 0; i < M; ++i) {
                for(int j = 0; j < N; ++j) {
                    for(int k = 0; k < N; ++k) {
                        data[i][j] += A.data[i][j] * B.data[j][k];
                    }
                }
            }
            return *this;
        }
        Matrix<T, M, N>& operator*=(const T B) {
            for(int i = 0; i < M; ++i) {
                for(int j = 0; j < N; ++j) {
                    data[i][j] *= B;
                }
            }
            return *this;
        }
        Matrix<T, M, N> operator*(const T B) {
            Matrix<T, M, N> A(*this);
            for(int i = 0; i < M; ++i) {
                for(int j = 0; j < N; ++j) {
                    data[i][j] *= B;
                }
            }
            return A;
        }
        Matrix<T, M, N> operator-() {
            Matrix<T, M, N> op;
            op *= -1;
            return op;
        }
    };
    template<typename T, int M, int N, int O>
    Matrix<T, M, O> operator*(const T B, const Matrix<T, M, N>& A) {
        Matrix<T, M, O> op(A);
        op *= B;
        return op;
    }

    template<typename T, int M, int N, int O>
    Matrix<T, M, O> operator*(const Matrix<T, M, N>& A, const Matrix<T, N, O> B) {
        Matrix<T, M, O> out;
        memset(out.data, 0, sizeof(out.data));
        for(int i = 0; i < M; ++i) {
            for(int j = 0; j < O; ++j) {
                for(int k = 0; k < N; ++k) {
                    out.data[i][j] += A.data[i][j] * B.data[j][k];
                }
            }
        }
        return out;
    }
    template<typename T, int M, int N>
    Matrix<T, M, N> operator+(const Matrix<T, M, N>& A, const Matrix<T, M, N>& B) {
        Matrix<T, M, N> op(A);
        op += A;
        return op;
    }
    template<typename T, int M, int N>
    Matrix<T, M, N> operator-(const Matrix<T, M, N>& A, const Matrix<T, M, N>& B) {
        Matrix<T, M, N> op(A);
        op -= B;
        return op;
    }
} // namespace syrup
#endif
