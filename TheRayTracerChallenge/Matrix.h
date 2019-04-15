#pragma once
#include "Tuple.h"
#include "Constants.h"
#include <array>
#include <iostream>

template <size_t nRows, size_t nColumns> class MatrixTemplate {
public:
    //The matrix's dimensions
    const double rows = nRows, columns = nColumns;

    //All stored values
    std::array<std::array<double, nColumns>, nRows> elements;

	MatrixTemplate() {
		for (size_t row = 0; row < rows; row++) {
			for (size_t column = 0; column < columns; column++) {
				elements[row][column] = 0.0;
			}
		}
	}
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
                if (!Constants::DoubleEqual(m.elements[row][column], elements[row][column]))
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
    inline double Minor(size_t row, size_t column) {
        auto submatrix = Submatrix(row, column);
        return submatrix.Determinant();
    }

    //Cofactor
    inline double Cofactor(size_t row, size_t column) {
        double minor = Minor(row, column);

        //The sign needs to be switched if row + column is an odd number
        return ((row + column) % 2 != 0) ?
            (-minor) :
            (minor);
    }

    //Determinant of a matrix of any size
    inline double Determinant() {
        double determinant = 0.0f;

        for (size_t column = 0; column < columns; column++) {
            determinant += elements[0][column] * Cofactor(0, column);
        }

        return determinant;
    }

    bool IsInvertible() {
        return !Constants::DoubleEqual(Determinant(), 0.0f);
    }

    inline MatrixTemplate<nRows, nColumns> Inversion() {
		double det = Determinant();
		
		if (Constants::DoubleEqual(det, 0.0f)) {
            std::cerr << "Matrix not invertible\n";
            return MatrixTemplate<nRows, nColumns>();
        }


        
        MatrixTemplate<nRows, nColumns> inversion;


        for (size_t row = 0; row < rows; row++) {
            for (size_t column = 0; column < columns; column++) {
                double c = Cofactor(row, column);

                //switching column and row accomplishes the transpose operation
                inversion.elements[column][row] = c / det;
            }
        }
        return inversion;
    }
};

using Matrix4x4 = MatrixTemplate<4, 4>;
using Matrix3x3 = MatrixTemplate<3, 3>;
using Matrix2x2 = MatrixTemplate<2, 2>;



namespace Matrix {

    //Initializing a 2x2 matrix
    inline Matrix2x2 Create(double a, double b,
        double c, double d) {

        Matrix2x2 matrix;

        matrix.elements[0][0] = a; matrix.elements[0][1] = b;
        matrix.elements[1][0] = c; matrix.elements[1][1] = d;

        return matrix;
    }

    //Initializing a 3x3 matrix
    inline Matrix3x3 Create(double a, double b, double c,
        double d, double e, double f,
        double g, double h, double i) {

        Matrix3x3 matrix;

        matrix.elements[0][0] = a; matrix.elements[0][1] = b; matrix.elements[0][2] = c;
        matrix.elements[1][0] = d; matrix.elements[1][1] = e; matrix.elements[1][2] = f;
        matrix.elements[2][0] = g; matrix.elements[2][1] = h; matrix.elements[2][2] = i;

        return matrix;
    }

    //Initializing a 4x4 matrix
    inline Matrix4x4 Create(double a, double b, double c, double d,
        double e, double f, double g, double h,
        double i, double j, double k, double l,
        double m, double n, double o, double p) {

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




//Template specialization for 2x2 matrices
template <>
inline double MatrixTemplate<2, 2>::Determinant() {
	return elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1];
}

//More efficient inversion for 4x4 matrices
template <>
inline Matrix4x4 MatrixTemplate<4, 4>::Inversion() {
	//This optimization speeds the rendering process up by about 96% (Compared to the unspecialized template)
	//Source: https://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform (Adapted to c++)
	
	
	double s0 = elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1];
	double s1 = elements[0][0] * elements[1][2] - elements[1][0] * elements[0][2];
	double s2 = elements[0][0] * elements[1][3] - elements[1][0] * elements[0][3];
	double s3 = elements[0][1] * elements[1][2] - elements[1][1] * elements[0][2];
	double s4 = elements[0][1] * elements[1][3] - elements[1][1] * elements[0][3];
	double s5 = elements[0][2] * elements[1][3] - elements[1][2] * elements[0][3];

	double c5 = elements[2][2] * elements[3][3] - elements[3][2] * elements[2][3];
	double c4 = elements[2][1] * elements[3][3] - elements[3][1] * elements[2][3];
	double c3 = elements[2][1] * elements[3][2] - elements[3][1] * elements[2][2];
	double c2 = elements[2][0] * elements[3][3] - elements[3][0] * elements[2][3];
	double c1 = elements[2][0] * elements[3][2] - elements[3][0] * elements[2][2];
	double c0 = elements[2][0] * elements[3][1] - elements[3][0] * elements[2][1];


	double det = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
	
	//Not invertible
	if (Constants::DoubleEqual(det, 0.0f)) {
		std::cerr << "Matrix not invertible\n";
		return Matrix4x4();
	}

	double invdet = 1.0f / det;


	return Matrix::Create(
		(elements[1][1] * c5 - elements[1][2] * c4 + elements[1][3] * c3) * invdet,
	(-elements[0][1] * c5 + elements[0][2] * c4 - elements[0][3] * c3) * invdet,
	(elements[3][1] * s5 - elements[3][2] * s4 + elements[3][3] * s3) * invdet,
	(-elements[2][1] * s5 + elements[2][2] * s4 - elements[2][3] * s3) * invdet,

	(-elements[1][0] * c5 + elements[1][2] * c2 - elements[1][3] * c1) * invdet,
	(elements[0][0] * c5 - elements[0][2] * c2 + elements[0][3] * c1) * invdet,
	(-elements[3][0] * s5 + elements[3][2] * s2 - elements[3][3] * s1) * invdet,
	(elements[2][0] * s5 - elements[2][2] * s2 + elements[2][3] * s1) * invdet,

	(elements[1][0] * c4 - elements[1][1] * c2 + elements[1][3] * c0) * invdet,
	(-elements[0][0] * c4 + elements[0][1] * c2 - elements[0][3] * c0) * invdet,
	(elements[3][0] * s4 - elements[3][1] * s2 + elements[3][3] * s0) * invdet,
	(-elements[2][0] * s4 + elements[2][1] * s2 - elements[2][3] * s0) * invdet,

	(-elements[1][0] * c3 + elements[1][1] * c1 - elements[1][2] * c0) * invdet,
	(elements[0][0] * c3 - elements[0][1] * c1 + elements[0][2] * c0) * invdet,
	(-elements[3][0] * s3 + elements[3][1] * s1 - elements[3][2] * s0) * invdet,
	(elements[2][0] * s3 - elements[2][1] * s1 + elements[2][2] * s0) * invdet
	);
}
