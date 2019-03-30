#pragma once
#include "Constants.h"
#include "Tuple.h"

class Matrix2x2
{
public:
    static const size_t rows = 2, columns = 2;
    float elements[rows][columns];

    Matrix2x2(float a, float b, 
        float c, float d);
    Matrix2x2() = default;
    ~Matrix2x2() = default;


    bool operator==(Matrix2x2 matrix);
    bool operator!=(Matrix2x2 matrix);
};

class Matrix3x3
{
public:
    static const size_t rows = 3, columns = 3;
    float elements[rows][columns];

    Matrix3x3(float a, float b, float c, 
        float d, float e, float f, 
        float g, float h, float i);
    Matrix3x3() = default;
    ~Matrix3x3() = default;

    bool operator==(Matrix3x3 matrix);
    bool operator!=(Matrix3x3 matrix);
};

class Matrix4x4
{
public:
    static const size_t rows = 4, columns = 4;
    float elements[rows][columns];

    Matrix4x4(float a, float b, float c, float d,
        float e, float f, float g, float h,
        float i, float j, float k, float l,
        float m, float n, float o, float p);
    Matrix4x4() = default;
    ~Matrix4x4() = default;

    bool operator==(Matrix4x4 matrix);
    bool operator!=(Matrix4x4 matrix);
    
    Tuple operator*(Tuple tuple);
    Matrix4x4 operator*(Matrix4x4 matrix);

    static Matrix4x4 IdentityMatrix();
};


//Constructors with initial values

inline Matrix2x2::Matrix2x2(float a, float b,
    float c, float d) {

    elements[0][0] = a; elements[0][1] = b;
    elements[1][0] = c; elements[1][1] = d;
}

inline Matrix3x3::Matrix3x3(float a, float b, float c,
    float d, float e, float f,
    float g, float h, float i) {

    elements[0][0] = a; elements[0][1] = b; elements[0][2] = c;
    elements[1][0] = d; elements[1][1] = e; elements[1][2] = f;
    elements[2][0] = g; elements[2][1] = h; elements[2][2] = i;
}

inline Matrix4x4::Matrix4x4(float a, float b, float c, float d,
    float e, float f, float g, float h,
    float i, float j, float k, float l,
    float m, float n, float o, float p) {

    elements[0][0] = a; elements[0][1] = b; elements[0][2] = c; elements[0][3] = d;
    elements[1][0] = e; elements[1][1] = f; elements[1][2] = g; elements[1][3] = h;
    elements[2][0] = i; elements[2][1] = j; elements[2][2] = k; elements[2][3] = l;
    elements[3][0] = m; elements[3][1] = n; elements[3][2] = o; elements[3][3] = p;
}


//Comparison operators

inline bool Matrix2x2::operator==(Matrix2x2 matrix) {
    return
        Constants::FloatEqual(elements[0][0], matrix.elements[0][0])
        && Constants::FloatEqual(elements[0][1], matrix.elements[0][1])

        && Constants::FloatEqual(elements[1][0], matrix.elements[1][0])
        && Constants::FloatEqual(elements[1][1], matrix.elements[1][1]);
}

inline bool Matrix2x2::operator!=(Matrix2x2 matrix) {
    return !(*this == matrix);
}

inline bool Matrix3x3::operator==(Matrix3x3 matrix) {
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            //Not equal, if any element is different
            if (!Constants::FloatEqual(elements[row][column], matrix.elements[row][column]))
                return false;

    //All elements are equal
    return true;
}

inline bool Matrix3x3::operator!=(Matrix3x3 matrix) {
    return !(*this == matrix);
}

inline bool Matrix4x4::operator==(Matrix4x4 matrix) {
    for (size_t row = 0; row < rows; row++)
        for (size_t column = 0; column < columns; column++)
            //Not equal, if any element is different
            if (!Constants::FloatEqual(elements[row][column], matrix.elements[row][column]))
                return false;

    //All elements are equal
    return true;
}

inline bool Matrix4x4::operator!=(Matrix4x4 matrix) {
    return !(*this == matrix);
}


//Mathematic operations

inline Tuple Matrix4x4::operator*(Tuple tuple) {
    Tuple result;

    result.x = elements[0][0] * tuple.x + elements[0][1] * tuple.y + elements[0][2] * tuple.z + elements[0][3] * tuple.w;
    result.y = elements[1][0] * tuple.x + elements[1][1] * tuple.y + elements[1][2] * tuple.z + elements[1][3] * tuple.w;
    result.z = elements[2][0] * tuple.x + elements[2][1] * tuple.y + elements[2][2] * tuple.z + elements[2][3] * tuple.w;
    result.w = elements[3][0] * tuple.x + elements[3][1] * tuple.y + elements[3][2] * tuple.z + elements[3][3] * tuple.w;

    return result;
}

inline Matrix4x4 Matrix4x4::operator*(Matrix4x4 matrix) {
    Matrix4x4 result;

    for (size_t row = 0; row < rows; row++) {
        for (size_t column = 0; column < columns; column++) {
            result.elements[row][column] =
                elements[row][0] * matrix.elements[0][column]
                + elements[row][1] * matrix.elements[1][column]
                + elements[row][2] * matrix.elements[2][column]
                + elements[row][3] * matrix.elements[3][column];
        }
    }

    return result;
}

inline Matrix4x4 Matrix4x4::IdentityMatrix() {
    return Matrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}