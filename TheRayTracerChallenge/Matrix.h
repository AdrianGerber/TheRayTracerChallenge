#pragma once
#include "Tuple.h"
#include "Constants.h"
#include <array>
#include <iostream>

template <size_t nRows, size_t nColumns> class MatrixTemplate {
public:
    //The matrix's dimensions
    const float rows = nRows, columns = nColumns;

    //All stored values
    std::array<std::array<float, nColumns>, nRows> elements;

    MatrixTemplate() = default;
    ~MatrixTemplate() = default;

    //Creating a smaller matrix by removing a row and a column
    MatrixTemplate<nRows - 1, nColumns - 1> Submatrix(size_t removedRow, size_t removedColumn) {
        MatrixTemplate < nRows - 1, nColumns - 1> submatrix;

        //Current position inside the submatrix
        size_t newRow = 0, newColumn = 0;

        //Go through all rows of the original matrix
        for (size_t row = 0; row < rows; row++) {

            //Skip the specified row
            if (row != removedRow) {
                newColumn = 0;
                //Go through all collumns of the original matrix
                for (size_t column = 0; column < columns; column++) {
                    //Skip the specified column
                    if (column != removedColumn) {
                        //Copy the elements to the submatrix
                        submatrix.elements[newRow][newColumn] = elements[row][column];
                        newColumn++;
                    }
                }
                //A row was copied -> next row inside submatrix
                newRow++;
            }
        }
        return submatrix;
    }

    //Comparison operators
    bool operator==(MatrixTemplate<nRows, nColumns> m) {
        for (size_t row = 0; row < rows; row++)
            for (size_t column = 0; column < columns; column++)
                //Not equal, if any element is different
                if (!Constants::FloatEqual(m.elements[row][column], elements[row][column]))
                    return false;
        //All elements are equal
        return true;
    }

    bool operator!=(MatrixTemplate<nRows, nColumns> m) {
        return !(*this == m);
    }

    void operator= (MatrixTemplate<nRows, nColumns> matrix) {
        elements = matrix.elements;
    }

    //Transposing a matrix
    inline MatrixTemplate<nRows, nColumns> Transpose() {
        MatrixTemplate<nRows, nColumns> result;

        //Switch rows and columns
        for (size_t row = 0; row < rows; row++) {
            for (size_t column = 0; column < columns; column++) {
                result.elements[column][row] = elements[row][column];
            }
        }
        return result;
    }


    //Minor
    inline float Minor(size_t row, size_t column) {
        auto submatrix = Submatrix(row, column);
        return submatrix.Determinant();
    }

    //Cofactor
    inline float Cofactor(size_t row, size_t column) {
        float minor = Minor(row, column);

        //The sign needs to be switched if row + column is an odd number
        return ((row + column) % 2 != 0) ?
            (-minor) :
            (minor);
    }

    //Determinant of a matrix of any size
    inline float Determinant() {
        float determinant = 0.0f;

        for (size_t column = 0; column < columns; column++) {
            determinant += elements[0][column] * Cofactor(0, column);
        }

        return determinant;
    }

    bool IsInvertible() {
        return !Constants::FloatEqual(Determinant(), 0.0f);
    }

    inline MatrixTemplate<nRows, nColumns> Inversion() {
        if (!IsInvertible()) {
            std::cerr << "Matrix not invertible\n";
            return MatrixTemplate<nRows, nColumns>();
        }


        float det = Determinant();
        MatrixTemplate<nRows, nColumns> inversion;


        for (size_t row = 0; row < rows; row++) {
            for (size_t column = 0; column < columns; column++) {
                float c = Cofactor(row, column);

                //switching column and row accomplishes the transpose operation
                inversion.elements[column][row] = c / det;
            }
        }
        return inversion;
    }
};

//Template specialization for 2x2 matrices
template <>
inline float MatrixTemplate<2, 2>::Determinant() {
    return elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1];
}

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

    //Identity matrix
    inline Matrix4x4 IndentityMatrix4x4() {
        return Matrix::Create(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
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