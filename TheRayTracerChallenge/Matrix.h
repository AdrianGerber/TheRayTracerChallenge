#pragma once
#include "MatrixTemplate.h"
#include "Tuple.h"
#include "Constants.h"
#include <array>
#include <iostream>

using Matrix4x4 = MatrixTemplate<4, 4>;
using Matrix3x3 = MatrixTemplate<3, 3>;
using Matrix2x2 = MatrixTemplate<2, 2>;

namespace Matrix {

    //Initializing a 2x2 matrix
    inline Matrix2x2 Create(float a, float b,
        float c, float d) {

        Matrix2x2 matrix;

        matrix.elements[0][0] = a; matrix.elements[0][1] = b;
        matrix.elements[1][0] = c; matrix.elements[1][1] = d;

        return matrix;
    }

    //Initializing a 3x3 matrix
    inline Matrix3x3 Create(float a, float b, float c,
        float d, float e, float f,
        float g, float h, float i) {

        Matrix3x3 matrix;

        matrix.elements[0][0] = a; matrix.elements[0][1] = b; matrix.elements[0][2] = c;
        matrix.elements[1][0] = d; matrix.elements[1][1] = e; matrix.elements[1][2] = f;
        matrix.elements[2][0] = g; matrix.elements[2][1] = h; matrix.elements[2][2] = i;

        return matrix;
    }

    //Initializing a 4x4 matrix
    inline Matrix4x4 Create(float a, float b, float c, float d,
        float e, float f, float g, float h,
        float i, float j, float k, float l,
        float m, float n, float o, float p) {

        Matrix4x4 matrix;

        matrix.elements[0][0] = a; matrix.elements[0][1] = b; matrix.elements[0][2] = c; matrix.elements[0][3] = d;
        matrix.elements[1][0] = e; matrix.elements[1][1] = f; matrix.elements[1][2] = g; matrix.elements[1][3] = h;
        matrix.elements[2][0] = i; matrix.elements[2][1] = j; matrix.elements[2][2] = k; matrix.elements[2][3] = l;
        matrix.elements[3][0] = m; matrix.elements[3][1] = n; matrix.elements[3][2] = o; matrix.elements[3][3] = p;

        return matrix;
    }

    //Transposing a matrix
    template<typename T>
    inline T Transpose(T matrix) {
        T result;

        //Switch rows and columns
        for (size_t row = 0; row < matrix.rows; row++) {
            for (size_t column = 0; column < matrix.columns; column++) {
                result.elements[column][row] = matrix.elements[row][column];
            }
        }
        return result;
    }
    
    //Identity matrix
    inline Matrix4x4 IndentityMatrix4x4() {
        return Matrix::Create(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    //Determinant of a 2x2 Matrix
    inline float Determinant(Matrix2x2 matrix) {
        return matrix.elements[0][0] * matrix.elements[1][1] - matrix.elements[1][0] * matrix.elements[0][1];
    }

    //Minor
    template<typename T>
    inline float Minor(T matrix,  size_t row, size_t column) {
        return Determinant(matrix.Submatrix(row, column));
    }

    //Cofactor
    template<typename T>
    inline float Cofactor(T matrix, size_t row, size_t column) {
        float minor = Minor(matrix, row, column);

        //The sign needs to be switched if row + column is an odd number
        return ((row + column) % 2 != 0)?
            (-minor):
            (minor);
    }

    //Determinant of a matrix of any size
    template<typename T>
    inline float Determinant(T matrix) {
        float determinant = 0.0f;

        for (size_t column = 0; column < matrix.columns; column++) {
            determinant += matrix.elements[0][column] * Cofactor(matrix, 0, column);
        }

        return determinant;
    }

    template<typename T>
    bool IsInvertible(T matrix) {
        return !Constants::FloatEqual(Determinant(matrix), 0.0f);
    }

    template<typename T>
    inline T Invert(T matrix) {
        if (!IsInvertible(matrix)) {
            std::cerr << "Matrix not invertible\n";
            return T();
        }

        
        float det = Determinant(matrix);
        T inversion;


        for (size_t row = 0; row < matrix.rows; row++) {
            for (size_t column = 0; column < matrix.columns; column++) {
                float c = Cofactor(matrix, row, column);

                //switching column and row accomplishes the transpose operation
                inversion.elements[column][row] = c / det;
            }
        }
        return inversion;
    }

}


//Multiplication of matrices and tuples

inline Tuple operator*(Tuple tuple, Matrix4x4 matrix) {
    Tuple result;

    result.x = matrix.elements[0][0] * tuple.x + matrix.elements[0][1] * tuple.y + matrix.elements[0][2] * tuple.z + matrix.elements[0][3] * tuple.w;
    result.y = matrix.elements[1][0] * tuple.x + matrix.elements[1][1] * tuple.y + matrix.elements[1][2] * tuple.z + matrix.elements[1][3] * tuple.w;
    result.z = matrix.elements[2][0] * tuple.x + matrix.elements[2][1] * tuple.y + matrix.elements[2][2] * tuple.z + matrix.elements[2][3] * tuple.w;
    result.w = matrix.elements[3][0] * tuple.x + matrix.elements[3][1] * tuple.y + matrix.elements[3][2] * tuple.z + matrix.elements[3][3] * tuple.w;

    return result;
}

inline Tuple operator*(Matrix4x4 matrix, Tuple tuple) {
    return tuple * matrix;
}

inline Matrix4x4 operator*(Matrix4x4 matrix1, Matrix4x4 matrix2) {
    Matrix4x4 result;

    for (size_t row = 0; row < matrix1.rows; row++) {
        for (size_t column = 0; column < matrix1.columns; column++) {
            result.elements[row][column] =
                matrix1.elements[row][0] * matrix2.elements[0][column]
                + matrix1.elements[row][1] * matrix2.elements[1][column]
                + matrix1.elements[row][2] * matrix2.elements[2][column]
                + matrix1.elements[row][3] * matrix2.elements[3][column];
        }
    }

    return result;
}