#pragma once

#include <array>

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
};




