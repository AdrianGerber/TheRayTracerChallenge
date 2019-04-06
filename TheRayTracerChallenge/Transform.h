#pragma once
#include "Matrix.h"
#include <cmath>

class Transform {
public:
    Matrix4x4 matrix;

    Transform();
    Transform(Matrix4x4 initialMatrix);

    ~Transform() = default;

    //Invert this transform
    Transform& Invert();
    //Get the inversion of this transform
    Transform Inversion();

    //Transform& Translate(float x, float y, float z);

    //Creating Translations
    static Transform CreateTranslation(float x, float y, float z);

    static Transform CreateScale(float x, float y, float z);

    static Transform CreateRotationX(float angle);
    static Transform CreateRotationY(float angle);
    static Transform CreateRotationZ(float angle);

    static Transform CreateShear(float xy, float xz, float yx, float yz, float zx, float zy);

    //Applying translations to an existing translation
    Transform& Translate(float x, float y, float z);

    Transform& Scale(float x, float y, float z);

    Transform& RotateX(float angle);
    Transform& RotateY(float angle);
    Transform& RotateZ(float angle);

    Transform& Shear(float xy, float xz, float yx, float yz, float zx, float zy);
};


inline Transform::Transform() : matrix{ Matrix::IndentityMatrix4x4() } {

}

inline Transform::Transform(Matrix4x4 initialMatrix) {
    matrix = initialMatrix;
}


inline Transform& Transform::Invert() {
    matrix = matrix.Inversion();
    return *this;
}

inline Transform Transform::Inversion() {
    return Transform(matrix.Inversion());
}


//Creating Translations

inline Transform Transform::CreateTranslation(float x, float y, float z) {
    Transform t(Matrix::IndentityMatrix4x4());
    
    t.matrix.elements[0][3] = x;
    t.matrix.elements[1][3] = y;
    t.matrix.elements[2][3] = z;

    return t;
}

inline Transform Transform::CreateScale(float x, float y, float z) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = x;
    t.matrix.elements[1][1] = y;
    t.matrix.elements[2][2] = z;

    return t;
}

inline Transform Transform::CreateRotationX(float angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[1][1] = cosf(angle);
    t.matrix.elements[1][2] = -sinf(angle);
    t.matrix.elements[2][1] = sinf(angle);
    t.matrix.elements[2][2] = cosf(angle);

    return t;
}

inline Transform Transform::CreateRotationY(float angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = cosf(angle);
    t.matrix.elements[0][2] = sinf(angle);
    t.matrix.elements[2][0] = -sinf(angle);
    t.matrix.elements[2][2] = cosf(angle);

    return t;
}

inline Transform Transform::CreateRotationZ(float angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = cosf(angle);
    t.matrix.elements[0][1] = -sinf(angle);
    t.matrix.elements[1][0] = sinf(angle);
    t.matrix.elements[1][1] = cosf(angle);

    return t;
}

inline Transform Transform::CreateShear(float xy, float xz, float yx, float yz, float zx, float zy) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][1] = xy;
    t.matrix.elements[0][2] = xz;

    t.matrix.elements[1][0] = yx;
    t.matrix.elements[1][2] = yz;
    
    t.matrix.elements[2][0] = zx;
    t.matrix.elements[2][1] = zy;

    return t;
}


//Multiplication of transforms with matrices and tuples
inline Tuple operator*(Tuple tuple, Transform transform) {
    return tuple * transform.matrix;
}

inline Tuple operator*(Transform transform, Tuple tuple) {
    return transform.matrix * tuple;
}

inline Matrix4x4 operator*(Transform transform, Matrix4x4 matrix) {
    return transform.matrix * matrix;
}

inline Matrix4x4 operator*(Matrix4x4 matrix, Transform transform) {
    return matrix * transform.matrix;
}


//Applying translations to an existing translation

inline Transform& Transform::Translate(float x, float y, float z) {
    matrix = Transform::CreateTranslation(x, y, z) * matrix;

    return *this;
}

inline Transform& Transform::Scale(float x, float y, float z) {
    matrix = Transform::CreateScale(x, y, z) * matrix;

    return *this;
}

inline Transform& Transform::RotateX(float angle) {
    matrix = Transform::CreateRotationX(angle) * matrix;

    return *this;
}

inline Transform& Transform::RotateY(float angle) {
    matrix = Transform::CreateRotationY(angle) * matrix;

    return *this;
}

inline Transform& Transform::RotateZ(float angle) {
    matrix = Transform::CreateRotationZ(angle) * matrix;

    return *this;
}

inline Transform& Transform::Shear(float xy, float xz, float yx, float yz, float zx, float zy) {
    matrix = Transform::CreateShear(xy, xz, yx, yz, zx, zy) * matrix;

    return *this;
}