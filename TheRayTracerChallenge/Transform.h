#pragma once
#include "Matrix.h"
#include <cmath>

//Definition of transforms and implementation of basic operations on them
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

    //Creating Transforms
    static Transform CreateTranslation(double x, double y, double z);
    static Transform CreateScale(double x, double y, double z);
    static Transform CreateRotationX(double angle);
    static Transform CreateRotationY(double angle);
    static Transform CreateRotationZ(double angle);
    static Transform CreateShear(double xy, double xz, double yx, double yz, double zx, double zy);

    //Applying translations to an existing transform
    Transform& Translate(double x, double y, double z);
    Transform& Scale(double x, double y, double z);
    Transform& RotateX(double angle);
    Transform& RotateY(double angle);
    Transform& RotateZ(double angle);
    Transform& Shear(double xy, double xz, double yx, double yz, double zx, double zy);
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

//Creation of transforms

inline Transform Transform::CreateTranslation(double x, double y, double z) {
    Transform t(Matrix::IndentityMatrix4x4());
    
    t.matrix.elements[0][3] = x;
    t.matrix.elements[1][3] = y;
    t.matrix.elements[2][3] = z;

    return t;
}

inline Transform Transform::CreateScale(double x, double y, double z) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = x;
    t.matrix.elements[1][1] = y;
    t.matrix.elements[2][2] = z;

    return t;
}

inline Transform Transform::CreateRotationX(double angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[1][1] = cos(angle);
    t.matrix.elements[1][2] = -sin(angle);
    t.matrix.elements[2][1] = sin(angle);
    t.matrix.elements[2][2] = cos(angle);

    return t;
}

inline Transform Transform::CreateRotationY(double angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = cos(angle);
    t.matrix.elements[0][2] = sin(angle);
    t.matrix.elements[2][0] = -sin(angle);
    t.matrix.elements[2][2] = cos(angle);

    return t;
}

inline Transform Transform::CreateRotationZ(double angle) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][0] = cos(angle);
    t.matrix.elements[0][1] = -sin(angle);
    t.matrix.elements[1][0] = sin(angle);
    t.matrix.elements[1][1] = cos(angle);

    return t;
}

inline Transform Transform::CreateShear(double xy, double xz, double yx, double yz, double zx, double zy) {
    Transform t(Matrix::IndentityMatrix4x4());

    t.matrix.elements[0][1] = xy;
    t.matrix.elements[0][2] = xz;

    t.matrix.elements[1][0] = yx;
    t.matrix.elements[1][2] = yz;
    
    t.matrix.elements[2][0] = zx;
    t.matrix.elements[2][1] = zy;

    return t;
}

inline bool operator==(Transform transform1, Transform transform2) {
    return transform1.matrix == transform2.matrix;
}

inline bool operator!=(Transform transform1, Transform transform2) {
    return !(transform1 == transform2);
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

inline Transform operator*(Transform transform1, Transform transform2) {
    return Transform(transform1.matrix * transform2.matrix);
}

//Applying transformations to an existing transform

inline Transform& Transform::Translate(double x, double y, double z) {
    matrix = Transform::CreateTranslation(x, y, z) * matrix;

    return *this;
}

inline Transform& Transform::Scale(double x, double y, double z) {
    matrix = Transform::CreateScale(x, y, z) * matrix;

    return *this;
}

inline Transform& Transform::RotateX(double angle) {
    matrix = Transform::CreateRotationX(angle) * matrix;

    return *this;
}

inline Transform& Transform::RotateY(double angle) {
    matrix = Transform::CreateRotationY(angle) * matrix;

    return *this;
}

inline Transform& Transform::RotateZ(double angle) {
    matrix = Transform::CreateRotationZ(angle) * matrix;

    return *this;
}

inline Transform& Transform::Shear(double xy, double xz, double yx, double yz, double zx, double zy) {
    matrix = Transform::CreateShear(xy, xz, yx, yz, zx, zy) * matrix;

    return *this;
}